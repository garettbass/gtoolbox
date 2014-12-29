#if TEST
#include <catch.h>
#include <fstream>
#include <zna/source/thirdParty/gtoolbox/xtd/echo.h>
#include <zna/source/thirdParty/gtoolbox/xtd/filesystem/path.h>
#include <zna/source/thirdParty/gtoolbox/xtd/utility/scope_timer.h>

// json
#include <zna/source/thirdParty/gtoolbox/json/value.h>
#include <zna/source/thirdParty/gtoolbox/json/encoding/debug/debug_encoder.h>
#include <zna/source/thirdParty/gtoolbox/json/encoding/json/json_decoder.h>
#include <zna/source/thirdParty/gtoolbox/json/encoding/json/json_encoder.h>
#include <zna/source/thirdParty/gtoolbox/json/encoding/value/value_decoder.h>
#include <zna/source/thirdParty/gtoolbox/json/encoding/value/value_encoder.h>

struct file
{
    xtd::path path;

    std::string buffer;

    file(const xtd::path& path)
    : path(path)
    {
        assert(path.is_file());

        std::ifstream stream(path.c_str());

        assert(stream.is_open());

        stream.seekg(0, std::ios::end);

        const size_t length = stream.tellg();

        buffer.resize(length + 1, '\0');

        stream.seekg(0, std::ios::beg);

        stream.read(&buffer[0], buffer.size());

        buffer.data();
    }

    const char* data() const { return buffer.data(); }
};

static void json_files(std::vector<file>& files, const xtd::path& root)
{
    for (auto path : root)
    {
        if (path.is_file() and path.extension() == "json")
        {
            files.emplace_back(path);
        }

        if (path.is_directory())
        {
            json_files(files, path);

            continue;
        }
    }
}

static std::vector<file> json_files(const xtd::path& root)
{
    std::vector<file> files;

    json_files(files, root);

    return files;
}

static void load_via_json_value(const file& file)
{
    using namespace json;

    value root;

    value_encoder encoder(root);

    json_decoder(file.data()).decode(encoder);
}

TEST_CASE("encoding tests", "[cfg][encoding]")
{
    const xtd::path file_path(__FILE__);

    REQUIRE(file_path.exists());

    const xtd::path data_dir(file_path.parent().str() + "data");

    REQUIRE(data_dir.exists());

    const xtd::path sample_json_path(data_dir.str() + "sample.json");

    REQUIRE(sample_json_path.exists());

    std::cout << sample_json_path.str() << std::endl;

    file sample_json_file(sample_json_path);

    TIMED_SCOPE("load 'sample.json' via json::value")
    {
        using namespace json;

        value sample_json_value;

        value_encoder sample_json_encoder(sample_json_value);

        json_decoder(sample_json_file.data()).decode(sample_json_encoder);
    }

    std::vector<file> files;

    TIMED_SCOPE("load all JSON files")
    {
        files = json_files(data_dir);
    }

    xtd_echo(files.size());

    puts("-------------------------------------------------------------------");

    TIMED_SCOPE("load all JSON files via json::value")
    {
        for (auto& file : files) load_via_json_value(file);
    }
}

#endif // TEST
