// RUN: not mlir-opt %s -o - 2>&1 | FileCheck %s
// This test verifies that diagnostic handler can emit the call stack successfully.

// -----

// Emit the first available call stack in the fused location.
func @constant_out_of_range() {
  // CHECK: mysource1: error: 'std.constant' op requires attribute's type ('i64') to match op's return type ('i1')
  // CHECK-NEXT: mysource2: note: called from
  // CHECK-NEXT: mysource3: note: called from
  %x = "std.constant"() {value = 100} : () -> i1 loc(fused["bar", callsite("foo"("mysource1":0:0) at callsite("mysource2":1:0 at "mysource3":2:0))])
  return
}
