#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: zq <input> <output>\n";
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);

    if (!input)
    {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::ofstream output(argv[2], std::ios::binary);

    if (!output)
    {
        std::cerr << "Failed to open output file\n";
        return 1;
    }

    z_stream stream{};

    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK)
    {
        std::cerr << "Failed to initialize compression\n";
        return 1;
    }

    constexpr std::size_t CHUNK_SIZE = 16384;

    std::vector<unsigned char> in(CHUNK_SIZE);
    std::vector<unsigned char> out(CHUNK_SIZE);

    int result = Z_OK;

    do
    {
        input.read(
            reinterpret_cast<char*>(in.data()),
            static_cast<std::streamsize>(in.size())
        );

        const auto bytesRead = input.gcount();

        stream.next_in = in.data();
        stream.avail_in = static_cast<uInt>(bytesRead);

        const int flush = input.eof() ? Z_FINISH : Z_NO_FLUSH;

        do
        {
            stream.next_out = out.data();
            stream.avail_out = static_cast<uInt>(out.size());

            result = deflate(&stream, flush);

            if (result != Z_OK && result != Z_STREAM_END)
            {
                deflateEnd(&stream);
                std::cerr << "Compression failed\n";
                return 1;
            }

            const auto bytesWritten = out.size() - stream.avail_out;

            output.write(
                reinterpret_cast<const char*>(out.data()),
                static_cast<std::streamsize>(bytesWritten)
            );

            if (!output)
            {
                deflateEnd(&stream);
                std::cerr << "Failed to write output file\n";
                return 1;
            }
        } while (stream.avail_out == 0);

    } while (result != Z_STREAM_END);
        
      deflateEnd(&stream);

      std::cout << "Compression Complete!\n";

      return 0;
}