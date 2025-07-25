/* Copyright 2016 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_TSL_PLATFORM_NET_H_
#define TENSORFLOW_TSL_PLATFORM_NET_H_

namespace tsl {
namespace internal {

// Return a port number that is not currently bound to any TCP or UDP port.
// On success returns the assigned port number. Otherwise returns -1.
int PickUnusedPort();

// Same as PickUnusedPort(), but fails a CHECK() if a port can't be found. In
// that case, the error message is logged to FATAL.
int PickUnusedPortOrDie();

}  // namespace internal
}  // namespace tsl

#endif  // TENSORFLOW_TSL_PLATFORM_NET_H_
