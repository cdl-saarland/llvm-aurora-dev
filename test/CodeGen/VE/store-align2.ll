; RUN: llc < %s -mtriple=ve-unknown-unknown | FileCheck %s

@vi8 = common dso_local local_unnamed_addr global i8 0, align 2
@vi16 = common dso_local local_unnamed_addr global i16 0, align 2
@vi32 = common dso_local local_unnamed_addr global i32 0, align 2
@vi64 = common dso_local local_unnamed_addr global i64 0, align 2
@vi128 = common dso_local local_unnamed_addr global i128 0, align 2
@vf32 = common dso_local local_unnamed_addr global float 0.000000e+00, align 2
@vf64 = common dso_local local_unnamed_addr global double 0.000000e+00, align 2
@vf128 = common dso_local local_unnamed_addr global fp128 0xL00000000000000000000000000000000, align 2

; Function Attrs: norecurse nounwind readonly
define void @storef128(fp128* nocapture, fp128) {
; CHECK-LABEL: storef128:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s2, 8(,%s0)
; CHECK-NEXT:  st %s3, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store fp128 %1, fp128* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef64(double* nocapture, double) {
; CHECK-LABEL: storef64:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store double %1, double* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef32(float* nocapture, float) {
; CHECK-LABEL: storef32:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  stu %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store float %1, float* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei128(i128* nocapture, i128) {
; CHECK-LABEL: storei128:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s2, 8(,%s0)
; CHECK-NEXT:  st %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store i128 %1, i128* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei64(i64* nocapture, i64) {
; CHECK-LABEL: storei64:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store i64 %1, i64* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei32(i32* nocapture, i32) {
; CHECK-LABEL: storei32:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  stl %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store i32 %1, i32* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei32tr(i32* nocapture, i64) {
; CHECK-LABEL: storei32tr:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  stl %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  %3 = trunc i64 %1 to i32
  store i32 %3, i32* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei16(i16* nocapture, i16) {
; CHECK-LABEL: storei16:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st2b %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store i16 %1, i16* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei16tr(i16* nocapture, i64) {
; CHECK-LABEL: storei16tr:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st2b %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  %3 = trunc i64 %1 to i16
  store i16 %3, i16* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei8(i8* nocapture, i8) {
; CHECK-LABEL: storei8:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st1b %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  store i8 %1, i8* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei8tr(i8* nocapture, i64) {
; CHECK-LABEL: storei8tr:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st1b %s1, (,%s0)
; CHECK-NEXT:  or %s11, 0, %s9
  %3 = trunc i64 %1 to i8
  store i8 %3, i8* %0, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef128stk(fp128) {
; CHECK-LABEL: storef128stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s1, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  st %s0, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca fp128, align 2
  store fp128 %0, fp128* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef64stk(double) {
; CHECK-LABEL: storef64stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s0, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca double, align 2
  store double %0, double* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef32stk(float) {
; CHECK-LABEL: storef32stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  stu %s0, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca float, align 2
  store float %0, float* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei128stk(i128) {
; CHECK-LABEL: storei128stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s1, 184(,%s11)
; CHECK-NEXT:  st %s0, 176(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca i128, align 2
  store i128 %0, i128* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei64stk(i64) {
; CHECK-LABEL: storei64stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st %s0, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca i64, align 2
  store i64 %0, i64* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei32stk(i32) {
; CHECK-LABEL: storei32stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  stl %s0, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca i32, align 2
  store i32 %0, i32* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei16stk(i16) {
; CHECK-LABEL: storei16stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st2b %s0, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca i16, align 2
  store i16 %0, i16* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei8stk(i8) {
; CHECK-LABEL: storei8stk:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  st1b %s0, {{[0-9]+}}(,%s11)
; CHECK-NEXT:  or %s11, 0, %s9
  %addr = alloca i8, align 2
  store i8 %0, i8* %addr, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef128com(fp128) {
; CHECK-LABEL: storef128com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vf128@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vf128@hi(%s34)
; CHECK-NEXT:  st %s0, 8(,%s34)
; CHECK-NEXT:  st %s1, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store fp128 %0, fp128* @vf128, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef64com(double) {
; CHECK-LABEL: storef64com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vf64@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vf64@hi(%s34)
; CHECK-NEXT:  st %s0, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store double %0, double* @vf64, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storef32com(float) {
; CHECK-LABEL: storef32com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vf32@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vf32@hi(%s34)
; CHECK-NEXT:  stu %s0, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store float %0, float* @vf32, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei128com(i128) {
; CHECK-LABEL: storei128com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vi128@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vi128@hi(%s34)
; CHECK-NEXT:  st %s1, 8(,%s34)
; CHECK-NEXT:  st %s0, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store i128 %0, i128* @vi128, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei64com(i64) {
; CHECK-LABEL: storei64com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vi64@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vi64@hi(%s34)
; CHECK-NEXT:  st %s0, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store i64 %0, i64* @vi64, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei32com(i32) {
; CHECK-LABEL: storei32com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vi32@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vi32@hi(%s34)
; CHECK-NEXT:  stl %s0, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store i32 %0, i32* @vi32, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei16com(i16) {
; CHECK-LABEL: storei16com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vi16@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vi16@hi(%s34)
; CHECK-NEXT:  st2b %s0, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store i16 %0, i16* @vi16, align 2
  ret void
}

; Function Attrs: norecurse nounwind readonly
define void @storei8com(i8) {
; CHECK-LABEL: storei8com:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:  lea %s34, vi8@lo
; CHECK-NEXT:  and %s34, %s34, (32)0
; CHECK-NEXT:  lea.sl %s34, vi8@hi(%s34)
; CHECK-NEXT:  st1b %s0, (,%s34)
; CHECK-NEXT:  or %s11, 0, %s9
  store i8 %0, i8* @vi8, align 2
  ret void
}

