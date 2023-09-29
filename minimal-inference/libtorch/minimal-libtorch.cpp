/* ==========================================================================

Minimal LibTorch example from https://pytorch.org/tutorials/advanced/cpp_export.html
Licence: modified BSD

========================================================================== */

#include <torch/script.h>

#include <iostream>
#include <memory>

int main(int argc, const char* argv[]) {

    std::string filepath = MODELS_PATH;
    const std::string modelpath = filepath + "/model.pt";

    std::cout << "Loading model from " << modelpath << std::endl;

    torch::jit::script::Module module;
    try {
     // Deserialize the ScriptModule from a file using torch::jit::load().
        module = torch::jit::load(modelpath);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
        std::cout << e.what() << std::endl;
        return -1;
    }

    // std::cout << "ok\n";
    return 0;
}