//
// Created by valentin.ackva on 25.10.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_TESTTHREAD_H
#define NN_INFERENCE_TEMPLATE_TESTTHREAD_H

#include <JuceHeader.h>

class TestThread : public juce::Thread {
public:
    TestThread() : juce::Thread("TestThread") { }

    ~TestThread() override {
        stopThread(100);
    }

    void run() override {
    }

};

#endif //NN_INFERENCE_TEMPLATE_TESTTHREAD_H
