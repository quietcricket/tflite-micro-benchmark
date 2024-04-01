
#ifdef ARDUINO
#include <Arduino.h>
#endif

#ifdef __ZEPHYR__
#include <zephyr.h>
#endif

#ifdef __MBED__
#include "mbed.h"
#endif


#ifndef TWO_PI
#define TWO_PI 6.2831853071795864769252867665590057683943387987502
#endif

#include "model.h"
// #include <string>
#include <tensorflow/lite/micro/kernels/micro_ops.h>
#include <tensorflow/lite/micro/micro_mutable_op_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/micro/system_setup.h>
#include <tensorflow/lite/schema/schema_generated.h>

using namespace std;
namespace
{
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *input = nullptr;
  // Output is ignored
  // TfLiteTensor *output = nullptr;
  // Not the smallest size it can be (760) but it does not need to be that small
  constexpr int kTensorArenaSize = 2 * 1024;
  static uint8_t tensor_arena[kTensorArenaSize];
} /* namespace */

void log(string message)
{
#ifdef ARDUINO
  Serial.println(message.c_str());
#else
  printk("%s\n", message.c_str());
#endif
}

int get_millis()
{
#ifdef ARDUINO
  return millis();
#else
  return k_uptime_get_32();
#endif
}

void setup()
{
#ifdef ARDUINO 
  Serial.begin(9600);
#endif

  // Map the model into a usable data structure..
  model = tflite::GetModel(g_model);
  // Add operation used in the model
  static tflite::MicroMutableOpResolver<1> resolver;
  resolver.AddFullyConnected();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  interpreter->AllocateTensors();

  // Find out the exact tensor size
  // printf("Area used bytes: %d\n", interpreter->arena_used_bytes());
  input = interpreter->input(0);
  // Output is ignored
  // output = interpreter->output(0);

  int NUM_CYCLES = 1000000;
  int BATCH_SIZE = 1000;
  log("Running inference for" + to_string(NUM_CYCLES) + " cycles," + to_string(BATCH_SIZE) + " cycles per batch");
  uint32_t start_time = get_millis();
  uint32_t batch_start_time = start_time;
  for (int i = 0; i < NUM_CYCLES; i++)
  {
    float x = TWO_PI * (i % BATCH_SIZE) / BATCH_SIZE;
    /* Quantize the input from floating-point to integer */
    int8_t x_quantized = x / input->params.scale + input->params.zero_point;
    /* Place the quantized input in the model's input tensor */
    input->data.int8[0] = x_quantized;
    interpreter->Invoke();
    if (i % BATCH_SIZE == 0 && i != 0)
    {
      uint32_t t = get_millis();
      log(to_string(i / BATCH_SIZE) + ", " + to_string(t - batch_start_time));
      batch_start_time = t;
    }
  }
  log("Average time: " + to_string((get_millis() - start_time) * BATCH_SIZE / NUM_CYCLES));
}

/**
 * loop function for Arduino
 * Doesn't compile without it.
 * It doesn't do anything for mbed and probably the compiler strips it out because it is not used anywhere
 */
void loop()
{
}

#ifdef __ZEPHYR__
int main()
{
  setup();
  while (true)
  {
    loop();
  }
  return 0;
}
#endif