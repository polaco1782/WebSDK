#include "headers.hpp"

int main(int argc, char **argv)
{
    try
    {
        // instancia um novo app do GTK e conecta ao widget principal
        auto app = Gtk::Application::create(argc, argv, "websdk.gtk");
        WebSDK web;

        app->run(web);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}