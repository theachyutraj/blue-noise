#include "core/image.hpp"
#include "core/random.hpp"
#include "visualization/pgm.hpp"
#include "analysis/statistics.hpp"
#include "analysis/spatial.hpp"
#include "analysis/fourier.hpp"
#include "visualization/spectrum.hpp"
#include "analysis/spectrum.hpp"
#include "generation/void_cluster.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

void print_stats(const Image2D<float> &image, std::size_t radius = 10)
{
    std::cout << "Image dimensions: " << image.width() << " x " << image.height() << '\n';
    // Compute and print the mean and variance of the image.
    std::cout << "Mean: " << analysis::mean(image) << '\n';
    std::cout << "Variance: " << analysis::variance(image) << '\n';

    // Compute and print the covariance at various offsets.
    std::cout << "Covariance at (0, 0): "
              << analysis::covariance(image, 0, 0) << '\n';

    std::cout << "Covariance at (1, 0): "
              << analysis::covariance(image, 1, 0) << '\n';

    std::cout << "Covariance at (0, 1): "
              << analysis::covariance(image, 0, 1) << '\n';

    std::cout << "Covariance at (1, 1): "
              << analysis::covariance(image, 1, 1) << '\n';

    std::cout << "Covariance at (1, -1): "
              << analysis::covariance(image, 1, -1) << '\n';

    // Compute and print the radial covariance profile up to a radius (of 10 pixels).
    const auto profile = analysis::radial_covariance(image, radius);

    std::cout << "\nRadial covariance:\n";

    for (std::size_t r = 0; r < profile.size(); ++r)
    {
        std::cout << r << ", " << profile[r] << '\n';
    }

    // Compute and print the histogram with 10 bins.
    const auto bins = analysis::histogram(image, 10);

    std::cout << "\nHistogram:\n";

    for (std::size_t i = 0; i < bins.size(); ++i)
    {
        std::cout << "Bin " << i << ": " << bins[i] << '\n';
    }
}

void spectral_analysis(Image2D<float> &image, const std::string &experiment_path)
{

    // Compute the power spectrum and save it as a CSV file.
    const auto power = analysis::power_spectrum(image);

    std::ofstream spectrum_file(experiment_path + "/power_spectrum.csv");

    if (!spectrum_file)
    {
        throw std::runtime_error("Could not create power_spectrum.csv");
    }

    for (std::size_t y = 0; y < power.height(); ++y)
    {
        for (std::size_t x = 0; x < power.width(); ++x)
        {
            if (x > 0)
            {
                spectrum_file << ',';
            }

            spectrum_file << power.at(x, y);
        }

        spectrum_file << '\n';
    }

    std::cout << "Saved power_spectrum.csv\n";

    // Compute the centered power spectrum.
    const auto centered_power = analysis::center_spectrum(power);

    // Save the log-scaled power spectrum as a PGM image.
    const auto spectrum = visualization::spectrum_image(centered_power);
    visualization::save_pgm(spectrum, experiment_path + "/power_spectrum.pgm");

    std::cout << "Saved " << experiment_path << "/power_spectrum.pgm\n";

    const auto radial = analysis::radial_power_spectrum(centered_power);
    std::ofstream radial_file(experiment_path + "/radial_power.csv");

    for (std::size_t r = 0; r < radial.size(); ++r)
    {
        radial_file << r << ',' << radial[r] << '\n';
    }

    std::cout << "Saved " << experiment_path << "/radial_power.csv\n";
}

void white_noise_experiment()
{
    std::string experiment_path = "experiments/white_noise";

    // Create the output directory if it doesn't exist.
    std::filesystem::create_directories(experiment_path);

    std::size_t width = 256;
    std::size_t height = 256;
    Image2D<float> image(width, height, 0.0f);
    Random rng(42); // Fixed seed for reproducibility.

    // Fill the image with random values in the range [0, 1].
    for (std::size_t y = 0; y < height; y++)
        for (std::size_t x = 0; x < width; x++)
            image.at(x, y) = rng.uniform();

    print_stats(image);

    // Save the image as a PGM file.
    visualization::save_pgm(image, experiment_path + "/white_noise.pgm");

    std::cout << "Saved " << experiment_path << "/white_noise.pgm\n";

    // Compute the radial power spectrum and save it to a CSV file.
    spectral_analysis(image, experiment_path);
}

void blue_noise_experiment0()
{
    std::size_t width = 64;
    std::size_t height = 64;

    std::string experiment_path = "experiments/blue_noise";

    // Create the output directory if it doesn't exist.
    std::filesystem::create_directories(experiment_path);

    std::cout << "--------------------------------\n";
    std::cout << "Blue Noise Experiment 0\n";
    std::cout << "--------------------------------\n";

    // Fill the image with blue noise using a void-and-cluster algorithm.
    Image2D<int> ranks = generation::make_void_cluster_rank_mask(width, height, 42, 2.0f, 6);

    // Normalize the ranks to [0, 1] for visualization.
    Image2D<float> image(width, height, 0.0f);
    const float max_rank = static_cast<float>(width * height - 1);

    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            // Normalize the rank to [0, 1].
            image.at(x, y) = static_cast<float>(ranks.at(x, y)) / max_rank;
        }
    }

    // Save the image as a PGM file.
    visualization::save_pgm(image, experiment_path + "/mask.pgm");

    // print_stats(image, 4);
    spectral_analysis(image, experiment_path);

    std::cout << "Saved " << experiment_path << "/mask.pgm\n";
}

void blue_noise_experiment1()
{
    std::size_t width = 64;
    std::size_t height = 64;

    std::string experiment_path = "experiments/blue_noise_binary";

    // Create the output directory if it doesn't exist.
    std::filesystem::create_directories(experiment_path);

    std::cout << "--------------------------------\n";
    std::cout << "Blue Noise Experiment 1\n";
    std::cout << "--------------------------------\n";

    // Fill the image with blue noise using a void-and-cluster algorithm.
    Image2D<int> ranks = generation::make_void_cluster_rank_mask(width, height, 42, 2.0f, 6);

    // Normalize the ranks to [0, 1] for visualization.
    Image2D<float> image(width, height, 0.0f);
    const float max_rank = static_cast<float>(width * height - 1);

    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            // Normalize the rank to [0, 1].
            image.at(x, y) = static_cast<float>(ranks.at(x, y)) / max_rank;
        }
    }

    Image2D<float> binary(width, height, 0.0f);

    const float threshold = 0.1f;

    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            binary.at(x, y) = image.at(x, y) < threshold ? 1.0f : 0.0f;
        }
    }

    // Save the image as a PGM file.
    visualization::save_pgm(image, experiment_path + "/mask.pgm");
    visualization::save_pgm(binary, experiment_path + "/mask_binary.pgm");

    // print_stats(image, 4);
    // spectral_analysis(image, experiment_path);
    spectral_analysis(binary, experiment_path);

    std::cout << "Saved " << experiment_path << "/mask.pgm\n";
}

int main()
{
    // white_noise_experiment();
    blue_noise_experiment0();
    blue_noise_experiment1();
    return 0;
}