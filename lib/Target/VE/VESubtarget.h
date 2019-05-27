//===-- VESubtarget.h - Define Subtarget for the VE -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the VE specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VE_VESUBTARGET_H
#define LLVM_LIB_TARGET_VE_VESUBTARGET_H

#include "VEFrameLowering.h"
#include "VEISelLowering.h"
#include "VEInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "VEGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class VESubtarget : public VEGenSubtargetInfo {
  Triple TargetTriple;
  virtual void anchor();

  VEInstrInfo InstrInfo;
  VETargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;
  VEFrameLowering FrameLowering;

public:
  VESubtarget(const Triple &TT, const std::string &CPU,
              const std::string &FS, const TargetMachine &TM);

  const VEInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const VERegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const VETargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }

  bool enableMachineScheduler() const override;

  /// ParseSubtargetFeatures - Parses features string setting specified
  /// subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);
  VESubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS);

  /// Given a actual stack size as determined by FrameInfo, this function
  /// returns adjusted framesize which includes space for register window
  /// spills and arguments.
  int getAdjustedFrameSize(int stackSize) const;

  bool isTargetLinux() const { return TargetTriple.isOSLinux(); }
};

} // end namespace llvm

#endif
