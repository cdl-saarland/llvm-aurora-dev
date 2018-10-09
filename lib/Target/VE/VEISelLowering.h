//===-- VEISelLowering.h - VE DAG Lowering Interface ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that VE uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VE_VEISELLOWERING_H
#define LLVM_LIB_TARGET_VE_VEISELLOWERING_H

#include "VE.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
  class VESubtarget;

  namespace VEISD {
    enum NodeType : unsigned {
      FIRST_NUMBER = ISD::BUILTIN_OP_END,
      CMPICC,      // Compare two GPR operands, set icc+xcc.
      CMPFCC,      // Compare two FP operands, set fcc.
      BRICC,       // Branch to dest on icc condition
      BRXCC,       // Branch to dest on xcc condition (64-bit only).
      BRFCC,       // Branch to dest on fcc condition
      SELECT,
      SELECT_ICC,  // Select between two values using the current ICC flags.
      SELECT_XCC,  // Select between two values using the current XCC flags.
      SELECT_FCC,  // Select between two values using the current FCC flags.

      EH_SJLJ_SETJMP,  // builtin setjmp operation
      EH_SJLJ_LONGJMP, // builtin longjmp operation

      Hi, Lo,      // Hi/Lo operations, typically on a global address.

      FTOI,        // FP to Int within a FP register.
      ITOF,        // Int to FP within a FP register.
      FTOX,        // FP to Int64 within a FP register.
      XTOF,        // Int64 to FP within a FP register.

      MAX,
      MIN,
      FMAX,
      FMIN,

      GETFUNPLT,   // load function address through %plt insturction
      GETSTACKTOP, // retrieve address of stack top (first address of
                   // locals and temporaries)
      GETTLSADDR,  // load address for TLS access

      MEMBARRIER,  // Compiler barrier only; generate a no-op.

      CALL,        // A call instruction.
      RET_FLAG,    // Return with a flag operand.
      GLOBAL_BASE_REG, // Global base reg for PIC.
      FLUSHW,      // FLUSH register windows to stack.

      // Intrinsics
      INT_LVM,      // for int_ve_lvm_mmss and int_ve_lvm_MMss
      INT_SVM,      // for int_ve_svm_sms and int_ve_svm_sMs

      INT_ANDM,     // for int_ve_andm_mmm and int_ve_andm_MMM
      INT_ORM,      // for int_ve_orm_mmm and int_ve_orm_MMM
      INT_XORM,     // for int_ve_xorm_mmm and int_ve_xorm_MMM
      INT_EQVM,     // for int_ve_eqvm_mmm and int_ve_eqvm_MMM
      INT_NNDM,     // for int_ve_nndm_mmm and int_ve_nndm_MMM
      INT_NEGM,     // for int_ve_negm_mm and int_ve_negm_MM
      INT_PCVM,     // for int_ve_pcvm_sm
      INT_LZVM,     // for int_ve_lzvm_sm
      INT_TOVM,     // for int_ve_tovm_sm
      INT_VADDUL,   // for int_ve_vaddul_vvvmv and int_ve_vaddul_vsvmv
      INT_VSUBUL,   // for int_ve_vsubul_vvvmv and int_ve_vsubul_vsvmv
      INT_VCMPUL,   // for int_ve_vcmpul_vvvmv and int_ve_vcmpul_vsvmv
      INT_VADDUW,   // for int_ve_vadduw_vvvmv and int_ve_vadduw_vsvmv
      INT_VSUBUW,   // for int_ve_vsubuw_vvvmv and int_ve_vsubuw_vsvmv
      INT_VCMPUW,   // for int_ve_vcmpuw_vvvmv and int_ve_vcmpuw_vsvmv
      INT_VADDSWSX, // for int_ve_vaddswsx_vvvmv and int_ve_vaddswsx_vsvmv
      INT_VSUBSWSX, // for int_ve_vsubswsx_vvvmv and int_ve_vsubswsx_vsvmv
      INT_VCMPSWSX, // for int_ve_vcmpswsx_vvvmv and int_ve_vcmpswsx_vsvmv
      INT_VADDSWZX, // for int_ve_vaddswzx_vvvmv and int_ve_vaddswzx_vsvmv
      INT_VSUBSWZX, // for int_ve_vsubswzx_vvvmv and int_ve_vsubswzx_vsvmv
      INT_VCMPSWZX, // for int_ve_vcmpswzx_vvvmv and int_ve_vcmpswzx_vsvmv
      INT_VADDSL,   // for int_ve_vaddsl_vvvmv and int_ve_vaddsl_vsvmv
      INT_VSUBSL,   // for int_ve_vsubsl_vvvmv and int_ve_vsubsl_vsvmv
      INT_VCMPSL,   // for int_ve_vcmpsl_vvvmv and int_ve_vcmpsl_vsvmv
      INT_PVADDU,   // for int_ve_pvaddu_vvvMv and int_ve_pvaddu_vsvMv
      INT_PVSUBU,   // for int_ve_pvsubu_vvvMv and int_ve_pvsubu_vsvMv
      INT_PVCMPU,   // for int_ve_pvcmpu_vvvMv and int_ve_pvcmpu_vsvMv
      INT_PVADDS,   // for int_ve_pvadds_vvvMv and int_ve_pvadds_vsvMv
      INT_PVSUBS,   // for int_ve_pvsubs_vvvMv and int_ve_pvsubs_vsvMv
      INT_PVCMPS,   // for int_ve_pvcmps_vvvMv and int_ve_pvcmps_vsvMv
    };
  }

  class VETargetLowering : public TargetLowering {
    const VESubtarget *Subtarget;
  public:
    VETargetLowering(const TargetMachine &TM, const VESubtarget &STI);
    SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
    
    /// computeKnownBitsForTargetNode - Determine which of the bits specified
    /// in Mask are known to be either zero or one and return them in the
    /// KnownZero/KnownOne bitsets.
    void computeKnownBitsForTargetNode(const SDValue Op,
                                       KnownBits &Known,
                                       const APInt &DemandedElts,
                                       const SelectionDAG &DAG,
                                       unsigned Depth = 0) const override;

    MachineBasicBlock *
    EmitInstrWithCustomInserter(MachineInstr &MI,
                                MachineBasicBlock *MBB) const override;

    const char *getTargetNodeName(unsigned Opcode) const override;

#if 0
    SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;
#endif

    ConstraintType getConstraintType(StringRef Constraint) const override;
    ConstraintWeight
    getSingleConstraintMatchWeight(AsmOperandInfo &info,
                                   const char *constraint) const override;
    void LowerAsmOperandForConstraint(SDValue Op,
                                      std::string &Constraint,
                                      std::vector<SDValue> &Ops,
                                      SelectionDAG &DAG) const override;

    unsigned
    getInlineAsmMemConstraint(StringRef ConstraintCode) const override {
      if (ConstraintCode == "o")
        return InlineAsm::Constraint_o;
      return TargetLowering::getInlineAsmMemConstraint(ConstraintCode);
    }

    std::pair<unsigned, const TargetRegisterClass *>
    getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                 StringRef Constraint, MVT VT) const override;

    bool isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const override;
    MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override {
      return MVT::i32;
    }

    unsigned getRegisterByName(const char* RegName, EVT VT,
                               SelectionDAG &DAG) const override;

    /// Override to support customized stack guard loading.
    bool useLoadStackGuardNode() const override;
    void insertSSPDeclarations(Module &M) const override;

    /// getSetCCResultType - Return the ISD::SETCC ValueType
    EVT getSetCCResultType(const DataLayout &DL, LLVMContext &Context,
                           EVT VT) const override;

    SDValue
    LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                         const SmallVectorImpl<ISD::InputArg> &Ins,
                         const SDLoc &dl, SelectionDAG &DAG,
                         SmallVectorImpl<SDValue> &InVals) const override;
    SDValue LowerFormalArguments_64(SDValue Chain, CallingConv::ID CallConv,
                                    bool isVarArg,
                                    const SmallVectorImpl<ISD::InputArg> &Ins,
                                    const SDLoc &dl, SelectionDAG &DAG,
                                    SmallVectorImpl<SDValue> &InVals) const;

    SDValue
      LowerCall(TargetLowering::CallLoweringInfo &CLI,
                SmallVectorImpl<SDValue> &InVals) const override;
    SDValue LowerCall_64(TargetLowering::CallLoweringInfo &CLI,
                         SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        const SmallVectorImpl<SDValue> &OutVals,
                        const SDLoc &dl, SelectionDAG &DAG) const override;
    SDValue LowerReturn_64(SDValue Chain, CallingConv::ID CallConv,
                           bool IsVarArg,
                           const SmallVectorImpl<ISD::OutputArg> &Outs,
                           const SmallVectorImpl<SDValue> &OutVals,
                           const SDLoc &DL, SelectionDAG &DAG) const;

    SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerGlobalTLSAddress(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerToTLSGeneralDynamicModel(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerToTLSLocalExecModel(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerConstantPool(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerBlockAddress(SDValue Op, SelectionDAG &DAG) const;

    SDValue LowerEH_SJLJ_SETJMP(SDValue Op, SelectionDAG &DAG,
                                const VETargetLowering &TLI) const ;
    SDValue LowerEH_SJLJ_LONGJMP(SDValue Op, SelectionDAG &DAG,
                                 const VETargetLowering &TLI) const ;

    unsigned getSRetArgSize(SelectionDAG &DAG, SDValue Callee) const;
    SDValue withTargetFlags(SDValue Op, unsigned TF, SelectionDAG &DAG) const;
    SDValue makeHiLoPair(SDValue Op, unsigned HiTF, unsigned LoTF,
                         SelectionDAG &DAG) const;
    SDValue makeAddress(SDValue Op, SelectionDAG &DAG) const;

    SDValue LowerF128_LibCallArg(SDValue Chain, ArgListTy &Args, SDValue Arg,
                                 const SDLoc &DL, SelectionDAG &DAG) const;
    SDValue LowerF128Op(SDValue Op, SelectionDAG &DAG,
                        const char *LibFuncName,
                        unsigned numArgs) const;
    SDValue LowerF128Compare(SDValue LHS, SDValue RHS, unsigned &SPCC,
                             const SDLoc &DL, SelectionDAG &DAG) const;

    SDValue LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerINTRINSIC_WO_CHAIN(SDValue Op, SelectionDAG &DAG) const;

    SDValue LowerDYNAMIC_STACKALLOC(SDValue Op, SelectionDAG &DAG) const;

    SDValue LowerATOMIC_FENCE(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerATOMIC_LOAD(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerATOMIC_STORE(SDValue Op, SelectionDAG &DAG) const;

    bool ShouldShrinkFPConstant(EVT VT) const override {
      // Do not shrink FP constpool if VT == MVT::f128.
      // (ldd, call _Q_fdtoq) is more expensive than two ldds.
      return VT != MVT::f128;
    }

    bool shouldInsertFencesForAtomic(const Instruction *I) const override {
      // VE uses Release consistency, so need fence for each atomics.
      return true;
    }
    Instruction *emitLeadingFence(IRBuilder<> &Builder, Instruction *Inst,
                                  AtomicOrdering Ord) const override;
    Instruction *emitTrailingFence(IRBuilder<> &Builder, Instruction *Inst,
                                   AtomicOrdering Ord) const override;

    AtomicExpansionKind shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const override;

    void ReplaceNodeResults(SDNode *N,
                            SmallVectorImpl<SDValue>& Results,
                            SelectionDAG &DAG) const override;

    MachineBasicBlock *expandSelectCC(MachineInstr &MI, MachineBasicBlock *BB,
                                      unsigned BROpcode) const;
    MachineBasicBlock *emitEHSjLjSetJmp(MachineInstr &MI,
                                        MachineBasicBlock *MBB) const;
    MachineBasicBlock *emitEHSjLjLongJmp(MachineInstr &MI,
                                         MachineBasicBlock *MBB) const;
  };
} // end namespace llvm

#endif    // VE_ISELLOWERING_H
