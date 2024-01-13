#include <tensorflow/lite/micro/micro_mutable_op_resolver.h>
#include <tensorflow/lite/micro/micro_log.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/micro/system_setup.h>
#include <tensorflow/lite/schema/schema_generated.h>

#include "model_mnist.h" /* quantized model */
#include "images.h"

namespace
{
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *input = nullptr;
  TfLiteTensor *output = nullptr;
  int input_length;

  /* Create an area of memory to use for input, output, and intermediate arrays.
   * The size of this will depend on the model you're using, and may need to be
   * determined by experimentation.
   */
  constexpr int kTensorArenaSize = 30 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
} /* namespace */

int main(void)
{

  // Map the model into a usable data structure..
  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION)
  {
    MicroPrintf("Model schema version mismatch %d:%d", model->version(), TFLITE_SCHEMA_VERSION);
    return -1;
  }
  else
  {
    MicroPrintf("Model version: %d", model->version());
  }

  // This pulls in all the operation implementations we need.
  static tflite::MicroMutableOpResolver<6> resolver;
  resolver.AddConv2D();
  resolver.AddRelu();
  resolver.AddMaxPool2D();
  resolver.AddReshape();
  resolver.AddFullyConnected();
  resolver.AddSoftmax();
  MicroPrintf("resolver constructed");
  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;
  MicroPrintf("Interpreter constructed");

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk)
  {
    MicroPrintf("AllocateTensors() failed");
    return -1;
  }
  else
  {
    MicroPrintf("AllocateTensor() Success");
  }

  size_t used_size = interpreter->arena_used_bytes();
  MicroPrintf("Area used bytes: %d", used_size);
  input = interpreter->input(0);
  output = interpreter->output(0);

  MicroPrintf("Model input:");
  MicroPrintf("input->type: %s", input->type);
  MicroPrintf("dims->size: %s", input->dims->size);
  for (int n = 0; n < input->dims->size; ++n)
  {
    MicroPrintf("dims->data[n]: %d", input->dims->data[n]);
  }

  MicroPrintf("Model output:");
  MicroPrintf("dims->size: %d", output->dims->size);
  for (int n = 0; n < output->dims->size; ++n)
  {
    MicroPrintf("dims->data[n]: %d", output->dims->data[n]);
  }

  int width = 28, height = 28;

  for (int i = 0; i < width * height; ++i)
  {
    input->data.f[i] = (float)(image[i] / 255.0);
  }

  MicroPrintf("Do inference");
  // uint32_t start_time = k_uptime_get_32();
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk)
  {
    MicroPrintf("Invoke failed");
    return -1;
  }
  // uint32_t duration = k_uptime_get_32() - start_time;
  // MicroPrintf("Inference time = %l", duration);

  for (int n = 0; n < 10; ++n)
  {
    float value = output->data.f[n];
    MicroPrintf("%n %f", n, value);
  }
  return 0;
}
