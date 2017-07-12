#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "dao_generator.h"
#include "model_generator.h"


static void usage(...)
{
    printf("usage: \n"
           "    -h          [help]\r\n"
           "    -o          [parse mapper + model generate dao]\r\n"
           "    -u          [parse sql files generate model + mapper file]\r\n"
           "    -m          [generate multifile,default generate single]\r\n"
           "    -s path     [path to scan,default current dir]\r\n"
           "    -d path     [generate dao path,default current dir]\r\n"
           "    -c path     [generate mapper file path,default current dir]\r\n"
           "    -k path     [generate model file path,default current dir]\r\n"
           "    -j path     [generate create_tables.hpp file path,default current dir]\r\n\r\n\r\n");
}


int main(int argc, char *argv[])
{
    char ch;
    std::string source_filepath("./");
    std::string dao_path("./");
    std::string mapper_path("./");
    std::string model_path("./");
    std::string create_table_path("./");

    bool mutilfile = false;
    bool print = false;
    bool o = false;
    bool u = false;


    while ((ch = getopt(argc, argv, "pmouhs:d:c:k:j:")) > 0)
    {
        switch (ch)
        {
            case 'c':
                mapper_path = optarg;
                acl_make_dirs(optarg, 755);
                break;
            case 'k':
                model_path = optarg;
                acl_make_dirs(optarg, 755);
                break;
            case 'j':
                create_table_path = optarg;
                acl_make_dirs(optarg, 755);
                break;
            case 'o':
                o = true;
                break;
            case 'u':
                u = true;
                break;
            case 's':
                source_filepath = optarg;
                break;
            case 'd':
                dao_path = optarg;
                acl_make_dirs(optarg,755);
                break;
            case 'm':
                mutilfile = true;
                break;
            case 'p':
                print = true;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                break;
        }
    }

    if (u)
    {
        acl::model_generator gen;

        if (gen.parse(source_filepath))
        {
            if (mutilfile)
            {
                gen.gen_mappers(mapper_path);
                gen.gen_models(model_path);
            }
            else
            {
                gen.gen_mapper(mapper_path);
                gen.gen_model(model_path);
            }

        }
        gen.gen_create_tables(create_table_path);
    }
    if (o)
    {
        acl::dao_generator gen;

        if (!gen.parse_path(source_filepath))
        {
            printf("parse_file error,exit()\n");
            return -1;
        }
        if (print)
        {
            gen.print_entries();
        }

        if (mutilfile)
        {
            gen.gen_code_multi_files(dao_path);
        }
        else
        {
            gen.gen_code(dao_path);
        }
        try
        {
        }
        catch (const std::exception&e)
        {
            printf("gen code error:%s", e.what());
            return -1;
        }

        if (print)
        {
            gen.print_mappers();
        }
    }
    return 0;
}