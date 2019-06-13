; RUN: llc < %s -mtriple=ve-unknown-unknown | FileCheck %s

define x86_regcallcc <256 x float> @m5v256f32() {
; CHECK-LABEL: m5v256f32:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, 256
; CHECK-NEXT:  lea.sl %s35, -1063256064
; CHECK-NEXT:  lvl %s34
; CHECK-NEXT:  or %s34, 0, %s35
; CHECK-NEXT:  vbrdu %v0,%s34
; CHECK-NEXT:  or %s11, 0, %s9
  ret <256 x float> <
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0,
    float -5.0, float -5.0, float -5.0, float -5.0
  >
}

define x86_regcallcc <256 x double> @m5v256f64() {
; CHECK-LABEL: m5v256f64:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, 256
; CHECK-NEXT:  lvl %s34
; CHECK-NEXT:  lea.sl %s34, -1072431104
; CHECK-NEXT:  vbrd %v0,%s34
; CHECK-NEXT:  or %s11, 0, %s9
  ret <256 x double> <
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0,
    double -5.0, double -5.0, double -5.0, double -5.0
  >
}

