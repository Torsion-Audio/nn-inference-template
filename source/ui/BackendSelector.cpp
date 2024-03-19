#include "BackendSelector.h"

BackendSelector::BackendSelector(juce::AudioProcessorValueTreeState& state) : apvts(state) {
    backendList = PluginParameters::backendTypes;
    currentBackend = stringToBackend(PluginParameters::defaultBackend);
}

void BackendSelector::setBackend(int backendID) {
    juce::String type = backendList.getReference(backendID);
    auto newBackend = stringToBackend(type);

    if (newBackend != currentBackend) {
        currentBackend = newBackend;
        repaint();
    }
}

anira::InferenceBackend BackendSelector::getBackend() {
    return currentBackend;
}

void BackendSelector::paint(juce::Graphics &g) {
    auto currentBound = getBounds();

    switch (currentBackend) {
        case anira::TFLITE:
            backendTFLite->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
            break;
        case anira::LIBTORCH:
            backendLibTorch->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
            break;
        case anira::ONNX:
            backendONNX->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
            break;
    }

    switch (currentHover) {
        case TFLITE_HOVER:
            highlight->drawWithin(g, tfliteHighlightBounds.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
            break;
        case LIBTORCH_HOVER:
            highlight->drawWithin(g, libtorchHighlightBounds.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
            break;
        case ONNX_HOVER:
            highlight->drawWithin(g, onnxHighlightBounds.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
            break;
        case NONE:
            break;
    }

    // Debug: Show hover hit boxes
    // g.setColour(juce::Colours::red);
    // g.drawRect(tfliteBounds);
    // g.drawRect(libtorchBounds);
    // g.drawRect(onnxBounds);
}

void BackendSelector::resized() {
    tfliteBounds = juce::Rectangle<int>(90, 280, 340, 70);
    libtorchBounds = juce::Rectangle<int>(90, 355, 340, 50);
    onnxBounds = juce::Rectangle<int>(90, 411, 340, 55);

    tfliteHighlightBounds = juce::Rectangle<int>(0, 205, getWidth(), 150);
    libtorchHighlightBounds = juce::Rectangle<int>(0, 258, getWidth(), 150);
    onnxHighlightBounds = juce::Rectangle<int>(0, 320, getWidth(), 150);
}

void BackendSelector::mouseExit(const juce::MouseEvent &) {
    if (currentHover != NONE) {
        currentHover = NONE;
        repaint();
    }
}

void BackendSelector::mouseMove(const juce::MouseEvent &event) {
    auto pos = event.getPosition();

    if ( libtorchBounds.contains(pos) ) {
        currentHover = LIBTORCH_HOVER;
    } else if ( tfliteBounds.contains(pos) ) {
        currentHover = TFLITE_HOVER;
    } else if ( onnxBounds.contains(pos) ) {
        currentHover = ONNX_HOVER;
    } else if (currentHover != NONE) {
        currentHover = NONE;
    } else {
        return;
    }

    repaint();
}

void BackendSelector::mouseDown(const juce::MouseEvent &event) {
    auto pos = event.getPosition();
    if ( libtorchBounds.contains(pos) ) {
        currentBackend = anira::LIBTORCH;
    } else if ( tfliteBounds.contains(pos) ) {
        currentBackend = anira::TFLITE;
    } else if ( onnxBounds.contains(pos) ) {
        currentBackend = anira::ONNX;
    } else {
        getNextBackend();
    }

    backendChanged();

    repaint();
}

void BackendSelector::backendChanged() {
    juce::AudioParameterChoice* choice = nullptr;
    choice = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter (PluginParameters::BACKEND_TYPE_ID.getParamID()));
    *choice = getCurrentBackendID();
}

int BackendSelector::getCurrentBackendID() {
    return backendList.indexOf(backendToString(currentBackend));
}

void BackendSelector::getNextBackend() {
    switch (currentBackend) {
        case anira::TFLITE:
            currentBackend = anira::LIBTORCH;
            break;
        case anira::LIBTORCH:
            currentBackend = anira::ONNX;
            break;
        case anira::ONNX:
            currentBackend = anira::TFLITE;
            break;
    }
}

anira::InferenceBackend BackendSelector::stringToBackend(juce::String &backendStr) {
    if (backendStr == "TFLITE") return anira::TFLITE;
    if (backendStr == "LIBTORCH") return anira::LIBTORCH;
    if (backendStr == "ONNXRUNTIME") return anira::ONNX;
    throw std::invalid_argument("Invalid backend string");
}

juce::String BackendSelector::backendToString(anira::InferenceBackend backend) {
    switch (backend) {
        case anira::TFLITE:
            return "TFLITE";
        case anira::LIBTORCH:
            return "LIBTORCH";
        case anira::ONNX:
            return "ONNXRUNTIME";
        default:
            return "";
    }
}
