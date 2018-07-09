//===-- VEMCTargetDesc.cpp - VE Target Descriptions -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides VE specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "VEMCTargetDesc.h"
#include "InstPrinter/VEInstPrinter.h"
#include "VEMCAsmInfo.h"
#include "VETargetStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "VEGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "VEGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "VEGenRegisterInfo.inc"

static MCAsmInfo *createVEMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT) {
  MCAsmInfo *MAI = new VEELFMCAsmInfo(TT);
  unsigned Reg = MRI.getDwarfRegNum(VE::S11, true);
  MCCFIInstruction Inst = MCCFIInstruction::createDefCfa(nullptr, Reg, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstrInfo *createVEMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitVEMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createVEMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitVEMCRegisterInfo(X, VE::S10);
  return X;
}

static MCSubtargetInfo *
createVEMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  if (CPU.empty())
    CPU = "ve";
  return createVEMCSubtargetInfoImpl(TT, CPU, FS);
}

static MCTargetStreamer *
createObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI) {
  return new VETargetELFStreamer(S);
}

static MCTargetStreamer *createTargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrint,
                                                 bool isVerboseAsm) {
  return new VETargetAsmStreamer(S, OS);
}

static MCInstPrinter *createVEMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  return new VEInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializeVETargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(getTheVETarget(), createVEMCAsmInfo);

  for (Target *T : {&getTheVETarget()}) {
    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(*T, createVEMCInstrInfo);

    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(*T, createVEMCRegisterInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T, createVEMCSubtargetInfo);

    // Register the object target streamer.
    TargetRegistry::RegisterObjectTargetStreamer(*T,
                                                 createObjectTargetStreamer);

    // Register the asm streamer.
    TargetRegistry::RegisterAsmTargetStreamer(*T, createTargetAsmStreamer);

    // Register the MCInstPrinter
    TargetRegistry::RegisterMCInstPrinter(*T, createVEMCInstPrinter);
  }
}