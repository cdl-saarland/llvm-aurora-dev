; RUN: llc < %s -mtriple=ve-unknown-unknown | FileCheck %s

define zeroext i1 @setccaf(double, double) #0 {
; CHECK-LABEL: setccaf:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    or %s0, 0, (0)1
  %3 = fcmp false double %0, %1
  ret i1 %3
}

define zeroext i1 @setccat(double, double) #0 {
; CHECK-LABEL: setccat:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    or %s0, 1, (0)1
  %3 = fcmp true double %0, %1
  ret i1 %3
}

define zeroext i1 @setccoeq(double, double) #0 {
; CHECK-LABEL: setccoeq:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.eq %s0, (63)0, %s34
  %3 = fcmp oeq double %0, %1
  ret i1 %3
}

define zeroext i1 @setccone(double, double) #0 {
; CHECK-LABEL: setccone:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.ne %s0, (63)0, %s34
  %3 = fcmp one double %0, %1
  ret i1 %3
}

define zeroext i1 @setccogt(double, double) #0 {
; CHECK-LABEL: setccogt:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.gt %s0, (63)0, %s34
  %3 = fcmp ogt double %0, %1
  ret i1 %3
}

define zeroext i1 @setccoge(double, double) #0 {
; CHECK-LABEL: setccoge:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.ge %s0, (63)0, %s34
  %3 = fcmp oge double %0, %1
  ret i1 %3
}

define zeroext i1 @setccolt(double, double) #0 {
; CHECK-LABEL: setccolt:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.lt %s0, (63)0, %s34
  %3 = fcmp olt double %0, %1
  ret i1 %3
}

define zeroext i1 @setccole(double, double) #0 {
; CHECK-LABEL: setccole:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.le %s0, (63)0, %s34
  %3 = fcmp ole double %0, %1
  ret i1 %3
}

define zeroext i1 @setccord(double, double) #0 {
; CHECK-LABEL: setccord:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.num %s0, (63)0, %s34
  %3 = fcmp ord double %0, %1
  ret i1 %3
}

define zeroext i1 @setccuno(double, double) #0 {
; CHECK-LABEL: setccuno:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.nan %s0, (63)0, %s34
  %3 = fcmp uno double %0, %1
  ret i1 %3
}

define zeroext i1 @setccueq(double, double) #0 {
; CHECK-LABEL: setccueq:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.eqnan %s0, (63)0, %s34
  %3 = fcmp ueq double %0, %1
  ret i1 %3
}

define zeroext i1 @setccune(double, double) #0 {
; CHECK-LABEL: setccune:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.nenan %s0, (63)0, %s34
  %3 = fcmp une double %0, %1
  ret i1 %3
}

define zeroext i1 @setccugt(double, double) #0 {
; CHECK-LABEL: setccugt:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.gtnan %s0, (63)0, %s34
  %3 = fcmp ugt double %0, %1
  ret i1 %3
}

define zeroext i1 @setccuge(double, double) #0 {
; CHECK-LABEL: setccuge:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.genan %s0, (63)0, %s34
  %3 = fcmp uge double %0, %1
  ret i1 %3
}

define zeroext i1 @setccult(double, double) #0 {
; CHECK-LABEL: setccult:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.ltnan %s0, (63)0, %s34
  %3 = fcmp ult double %0, %1
  ret i1 %3
}

define zeroext i1 @setccule(double, double) #0 {
; CHECK-LABEL: setccule:
; CHECK:       .LBB{{[0-9]+}}_2:
; CHECK-NEXT:    fcmp.d %s34, %s0, %s1
; CHECK-NEXT:    or %s0, 0, (0)1
; CHECK-NEXT:    cmov.d.lenan %s0, (63)0, %s34
  %3 = fcmp ule double %0, %1
  ret i1 %3
}