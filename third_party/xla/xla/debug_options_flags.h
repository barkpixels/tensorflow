/* Copyright 2017 The OpenXLA Authors.

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

#ifndef XLA_DEBUG_OPTIONS_FLAGS_H_
#define XLA_DEBUG_OPTIONS_FLAGS_H_

#include <vector>

#include "absl/log/log.h"
#include "absl/strings/string_view.h"
#include "xla/tsl/platform/logging.h"
#include "xla/tsl/util/command_line_flags.h"
#include "xla/xla.pb.h"

namespace xla {

// Construct flags which write to the debug_options proto when parsed. Existing
// contents of debug_options is used as the default. Can be called multiple
// times.
void MakeDebugOptionsFlags(std::vector<tsl::Flag>* flag_list,
                           DebugOptions* debug_options);

// Appends flag definitions for debug options to flag_list. Existing
// contents of debug_options is used as the default. If debug_options is null,
// uses global defaults. Modifies global state on first call.
void AppendDebugOptionsFlags(std::vector<tsl::Flag>* flag_list,
                             DebugOptions* debug_options = nullptr);

// Parses the debug option flags from XLA_FLAGS environment variable. The
// global variable containing the debug options returned from
// 'GetDebugOptionsFromFlags' is mutated by setting fields explicitly specified
// in the environment variable. If `reset_envvar` is true, then the environment
// variable is read again, otherwise the previously read value is used.
void ParseDebugOptionFlagsFromEnv(bool reset_envvar);

// Parse the debug options from debug_options file. Given a string containing
// the textual form of a DebugOptions protobuf, parses it. The global variable
// containing the debug options returned from 'GetDebugOptionsFromFlags' is
// mutated by setting fields explicitly specified in the file.
bool ParseFlagsFromDebugOptionsFile(absl::string_view filename);

// Reset the flag values to default debug options ignoring flags.
void ResetFlagValues();

enum class FlagStatus {
  kStable,        // Can be changed/removed with 6 months deprecation notice.
  kDeprecated,    // Will stop working after 6 months of being deprecated.
  kExperimental,  // no guarantees, can be changed or removed without notice.
};
// All flags are considered experimental by default, which comes with no
// guarantees about backward compatibility.
//
// Policy for stable flags.
// 1. Should affect compilation results.
// 2. Should provide 6 months of deprecation notice before they stop working.
// 3. Should be used only for feature flags and not debugging flags.
//
// How to mark a flag stable:
// 1. Mark it as stable in the flag description, by prefixing it with
//    "[Stable]".
// 2. Add the flag name to the kStableFlags list in `GetFlagStatus` in
//    `debug_options_flags.cc`.
//
// How to deprecate a stable flag:
// 1. Mark it as deprecated in xla.proto.
// 2. Explain the reason and alternative in the flag description, and add prefix
//    "[Deprecated]" to the flag description.
// 3. Move it to the kDeprecatedFlags list in `GetFlagStatus` in
//    `debug_options_flags.cc`
// 4. After 6 months, the flag can be removed or disabled.
//
// Returns the status of the flag.
FlagStatus GetFlagStatus(absl::string_view flag_name);

// Fetches a DebugOptions proto message from flags provided to the program.
// Flags must be registered with the flags parser using AppendDebugOptionsFlags
// first.
DebugOptions GetDebugOptionsFromFlags();

// Gets a DebugOptions proto that reflects the defaults as if no flags were set.
DebugOptions DefaultDebugOptionsIgnoringFlags();

// Checks whether the pass fuel was explicitly set.
bool PassFuelIsSet(absl::string_view pass);

// Consumes a unit of "compiler fuel" for the given pass, and returns false if
// we're out of fuel for that pass.
//
// Compiler fuel is a debugging tool useful for bisecting compiler passes.  Each
// time a pass "does something", it consumes a unit of fuel, and once it's out
// of fuel, it stops doing any transformations.  This way if you suspect a pass
// has a bug, you can bisect the amount of fuel it gets and find exactly which
// change causes the problem.
//
// The very first time a pass runs out of fuel, `just_ran_out` is set to true.
// This lets you take action (e.g. log a message).  But see also the convenience
// overload below.
//
// By default all passes have infinite fuel.  You can restrict how much fuel a
// pass has by specifying XLA_FLAGS=--xla_fuel=PASS1=NUM1,PASS2=NUM2,...
//
// If a user specifies --xla_fuel=PASS=NUM but ConsumeFuel(PASS) is not called
// before the program exits, we'll print a warning.
//
// We recommend as a convention you use a pass's name for the `pass` argument,
// but any value is accepted.
bool ConsumeFuel(absl::string_view pass, bool* just_ran_out = nullptr);

// Overload of ConsumeFuel that lets you pass in a functor which generates a log
// message when we first run out of fuel for a pass.  This is useful because
// you're usually interested in what *would have* happened right when we ran out
// of fuel.
//
// Example usage:
//
//   if (ConsumeFuel("pass-name", [&] { return "Not fooing bar."; })) {
//     return;
//   }
//
template <typename MsgGenerator>
bool ConsumeFuel(absl::string_view pass,
                 const MsgGenerator& ran_out_of_fuel_msg) {
  bool just_ran_out = false;
  bool ret = ConsumeFuel(pass, &just_ran_out);
  if (just_ran_out) {
    LOG(ERROR) << "Out of fuel for \"" << pass
               << "\": " << ran_out_of_fuel_msg();
  }
  return ret;
}

// By default compiler fuel is global; if you run two compiler threads, they
// will consume from the same fuel pool.
//
// Calling this function changes the behavior of fuel for the current thread:
// From this point onward, it will use a private fuel pool.  The thread-local
// fuel pool is initialized to the values the global fuel pool had at process
// startup.
//
// You may call this function twice in the same thread to reset its fuel pool
// back to the initial state.
void ResetThreadLocalFuel();

}  // namespace xla

#endif  // XLA_DEBUG_OPTIONS_FLAGS_H_
