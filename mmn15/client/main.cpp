#include <iostream>
#include "Workflow.h"

int main()
{
    try {
        Workflow* workflow = new Workflow();
        std::cout << "Starting workflow" << std::endl;
        workflow->run();
        delete workflow;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error while running workflow: " << e.what() << std::endl;
        exit(1);
    }
}
