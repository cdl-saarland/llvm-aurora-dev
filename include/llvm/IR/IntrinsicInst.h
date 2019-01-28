//===-- llvm/IntrinsicInst.h - Intrinsic Instruction Wrappers ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines classes that make it really easy to deal with intrinsic
// functions with the isa/dyncast family of functions.  In particular, this
// allows you to do things like:
//
//     if (MemCpyInst *MCI = dyn_cast<MemCpyInst>(Inst))
//        ... MCI->getDest() ... MCI->getSource() ...
//
// All intrinsic function calls are instances of the call instruction, so these
// are all subclasses of the CallInst class.  Note that none of these classes
// has state or virtual methods, which is an important part of this gross/neat
// hack working.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_IR_INTRINSICINST_H
#define LLVM_IR_INTRINSICINST_H

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"
#include <cassert>
#include <cstdint>

namespace llvm {

  /// A wrapper class for inspecting calls to intrinsic functions.
  /// This allows the standard isa/dyncast/cast functionality to work with calls
  /// to intrinsic functions.
  class IntrinsicInst : public CallInst {
  public:
    IntrinsicInst() = delete;
    IntrinsicInst(const IntrinsicInst &) = delete;
    IntrinsicInst &operator=(const IntrinsicInst &) = delete;

    /// Return the intrinsic ID of this intrinsic.
    Intrinsic::ID getIntrinsicID() const {
      return getCalledFunction()->getIntrinsicID();
    }

    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const CallInst *I) {
      if (const Function *CF = I->getCalledFunction())
        return CF->isIntrinsic();
      return false;
    }
    static bool classof(const Value *V) {
      return isa<CallInst>(V) && classof(cast<CallInst>(V));
    }
  };

  /// This is the common base class for debug info intrinsics.
  class DbgInfoIntrinsic : public IntrinsicInst {
  public:
    /// \name Casting methods
    /// @{
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::dbg_declare:
      case Intrinsic::dbg_value:
      case Intrinsic::dbg_addr:
      case Intrinsic::dbg_label:
        return true;
      default: return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
    /// @}
  };

  /// This is the common base class for debug info intrinsics for variables.
  class DbgVariableIntrinsic : public DbgInfoIntrinsic {
  public:
    /// Get the location corresponding to the variable referenced by the debug
    /// info intrinsic.  Depending on the intrinsic, this could be the
    /// variable's value or its address.
    Value *getVariableLocation(bool AllowNullOp = true) const;

    /// Does this describe the address of a local variable. True for dbg.addr
    /// and dbg.declare, but not dbg.value, which describes its value.
    bool isAddressOfVariable() const {
      return getIntrinsicID() != Intrinsic::dbg_value;
    }

    DILocalVariable *getVariable() const {
      return cast<DILocalVariable>(getRawVariable());
    }

    DIExpression *getExpression() const {
      return cast<DIExpression>(getRawExpression());
    }

    Metadata *getRawVariable() const {
      return cast<MetadataAsValue>(getArgOperand(1))->getMetadata();
    }

    Metadata *getRawExpression() const {
      return cast<MetadataAsValue>(getArgOperand(2))->getMetadata();
    }

    /// Get the size (in bits) of the variable, or fragment of the variable that
    /// is described.
    Optional<uint64_t> getFragmentSizeInBits() const;

    /// \name Casting methods
    /// @{
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::dbg_declare:
      case Intrinsic::dbg_value:
      case Intrinsic::dbg_addr:
        return true;
      default: return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
    /// @}
  };

  /// This represents the llvm.dbg.declare instruction.
  class DbgDeclareInst : public DbgVariableIntrinsic {
  public:
    Value *getAddress() const { return getVariableLocation(); }

    /// \name Casting methods
    /// @{
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::dbg_declare;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
    /// @}
  };

  /// This represents the llvm.dbg.addr instruction.
  class DbgAddrIntrinsic : public DbgVariableIntrinsic {
  public:
    Value *getAddress() const { return getVariableLocation(); }

    /// \name Casting methods
    /// @{
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::dbg_addr;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This represents the llvm.dbg.value instruction.
  class DbgValueInst : public DbgVariableIntrinsic {
  public:
    Value *getValue() const {
      return getVariableLocation(/* AllowNullOp = */ false);
    }

    /// \name Casting methods
    /// @{
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::dbg_value;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
    /// @}
  };

  /// This represents the llvm.dbg.label instruction.
  class DbgLabelInst : public DbgInfoIntrinsic {
  public:
    DILabel *getLabel() const {
      return cast<DILabel>(getRawLabel());
    }

    Metadata *getRawLabel() const {
      return cast<MetadataAsValue>(getArgOperand(0))->getMetadata();
    }

    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    /// @{
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::dbg_label;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
    /// @}
  };

  class EVLIntrinsic : public IntrinsicInst {
  public:

    bool isUnaryOp() const;
    bool isBinaryOp() const;
    bool isTernaryOp() const;

    CmpInst::Predicate getCmpPredicate() const;

    Value* GetMask() const;
    Value* GetVectorLength() const;

    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      default:
        return false;

      case Intrinsic::evl_cmp:

      case Intrinsic::evl_and:
      case Intrinsic::evl_or:
      case Intrinsic::evl_xor:
      case Intrinsic::evl_ashr:
      case Intrinsic::evl_lshr:
      case Intrinsic::evl_shl:

      case Intrinsic::evl_select:
      case Intrinsic::evl_compose:
      case Intrinsic::evl_compress:
      case Intrinsic::evl_expand:
      case Intrinsic::evl_vshift:

      case Intrinsic::evl_load:
      case Intrinsic::evl_store:
      case Intrinsic::evl_gather:
      case Intrinsic::evl_scatter:

      case Intrinsic::evl_fneg:

      case Intrinsic::evl_fadd:
      case Intrinsic::evl_fsub:
      case Intrinsic::evl_fmul:
      case Intrinsic::evl_fdiv:
      case Intrinsic::evl_frem:

      case Intrinsic::evl_fma:

      case Intrinsic::evl_add:
      case Intrinsic::evl_sub:
      case Intrinsic::evl_mul:
      case Intrinsic::evl_udiv:
      case Intrinsic::evl_sdiv:
      case Intrinsic::evl_urem:
      case Intrinsic::evl_srem:

      case Intrinsic::evl_reduce_add:
      case Intrinsic::evl_reduce_mul:
      case Intrinsic::evl_reduce_umin:
      case Intrinsic::evl_reduce_umax:
      case Intrinsic::evl_reduce_smin:
      case Intrinsic::evl_reduce_smax:

      case Intrinsic::evl_reduce_and:
      case Intrinsic::evl_reduce_or:
      case Intrinsic::evl_reduce_xor:

      case Intrinsic::evl_reduce_fadd:
      case Intrinsic::evl_reduce_fmul:
      case Intrinsic::evl_reduce_fmin:
      case Intrinsic::evl_reduce_fmax:
        return true;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }

    unsigned getFunctionalOpcode() const {
      switch (getIntrinsicID()) {
        default: return getOpcode();

      case Intrinsic::evl_cmp:
        if (getArgOperand(0)->getType()->isFloatingPointTy()) {
          return Instruction::FCmp;
        } else {
          return Instruction::ICmp;
        }

      case Intrinsic::evl_and:  return Instruction::And;
      case Intrinsic::evl_or:   return Instruction::Or;
      case Intrinsic::evl_xor:  return Instruction::Xor;
      case Intrinsic::evl_ashr: return Instruction::AShr;
      case Intrinsic::evl_lshr: return Instruction::LShr;
      case Intrinsic::evl_shl:  return Instruction::Shl;

      case Intrinsic::evl_select: return Instruction::Select;
      // case Intrinsic::evl_compose:
      // case Intrinsic::evl_compress:
      // case Intrinsic::evl_expand:
      // case Intrinsic::evl_vshift:

      case Intrinsic::evl_load:   return Instruction::Load;
      case Intrinsic::evl_store:  return Instruction::Store;
      // case Intrinsic::evl_gather:
      // case Intrinsic::evl_scatter:

      case Intrinsic::evl_fneg:   return Instruction::FNeg;

      case Intrinsic::evl_fadd:   return Instruction::FAdd;
      case Intrinsic::evl_fsub:   return Instruction::FSub;
      case Intrinsic::evl_fmul:   return Instruction::FMul;
      case Intrinsic::evl_fdiv:   return Instruction::FDiv;
      case Intrinsic::evl_frem:   return Instruction::FRem;

      // case Intrinsic::evl_fma:

      case Intrinsic::evl_add:    return Instruction::Add;
      case Intrinsic::evl_sub:    return Instruction::Sub;
      case Intrinsic::evl_mul:    return Instruction::Mul;
      case Intrinsic::evl_udiv:   return Instruction::UDiv;
      case Intrinsic::evl_sdiv:   return Instruction::SDiv;
      case Intrinsic::evl_urem:   return Instruction::URem;
      case Intrinsic::evl_srem:   return Instruction::SRem;

      // case Intrinsic::evl_reduce_add:
      // case Intrinsic::evl_reduce_mul:
      // case Intrinsic::evl_reduce_umin:
      // case Intrinsic::evl_reduce_umax:
      // case Intrinsic::evl_reduce_smin:
      // case Intrinsic::evl_reduce_smax:

      // case Intrinsic::evl_reduce_and:
      // case Intrinsic::evl_reduce_or:
      // case Intrinsic::evl_reduce_xor:

      // case Intrinsic::evl_reduce_fadd:
      // case Intrinsic::evl_reduce_fmul:
      // case Intrinsic::evl_reduce_fmin:
      // case Intrinsic::evl_reduce_fmax:
      //   return true;
      }
    }
  };

  /// This is the common base class for constrained floating point intrinsics.
  class ConstrainedFPIntrinsic : public IntrinsicInst {
  public:
    enum RoundingMode {
      rmInvalid,
      rmDynamic,
      rmToNearest,
      rmDownward,
      rmUpward,
      rmTowardZero
    };

    enum ExceptionBehavior {
      ebInvalid,
      ebIgnore,
      ebMayTrap,
      ebStrict
    };

    bool isUnaryOp() const;
    bool isTernaryOp() const;
    RoundingMode getRoundingMode() const;
    ExceptionBehavior getExceptionBehavior() const;

    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::experimental_constrained_fadd:
      case Intrinsic::experimental_constrained_fsub:
      case Intrinsic::experimental_constrained_fmul:
      case Intrinsic::experimental_constrained_fdiv:
      case Intrinsic::experimental_constrained_frem:
      case Intrinsic::experimental_constrained_fma:
      case Intrinsic::experimental_constrained_sqrt:
      case Intrinsic::experimental_constrained_pow:
      case Intrinsic::experimental_constrained_powi:
      case Intrinsic::experimental_constrained_sin:
      case Intrinsic::experimental_constrained_cos:
      case Intrinsic::experimental_constrained_exp:
      case Intrinsic::experimental_constrained_exp2:
      case Intrinsic::experimental_constrained_log:
      case Intrinsic::experimental_constrained_log10:
      case Intrinsic::experimental_constrained_log2:
      case Intrinsic::experimental_constrained_rint:
      case Intrinsic::experimental_constrained_nearbyint:
      case Intrinsic::experimental_constrained_maxnum:
      case Intrinsic::experimental_constrained_minnum:
      case Intrinsic::experimental_constrained_ceil:
      case Intrinsic::experimental_constrained_floor:
      case Intrinsic::experimental_constrained_round:
      case Intrinsic::experimental_constrained_trunc:
        return true;
      default: return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class represents a op.with.overflow intrinsic.
  class WithOverflowInst : public IntrinsicInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::uadd_with_overflow:
      case Intrinsic::sadd_with_overflow:
      case Intrinsic::usub_with_overflow:
      case Intrinsic::ssub_with_overflow:
      case Intrinsic::umul_with_overflow:
      case Intrinsic::smul_with_overflow:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }

    Value *getLHS() const { return const_cast<Value*>(getArgOperand(0)); }
    Value *getRHS() const { return const_cast<Value*>(getArgOperand(1)); }

    /// Returns the binary operation underlying the intrinsic.
    Instruction::BinaryOps getBinaryOp() const;

    /// Whether the intrinsic is signed or unsigned.
    bool isSigned() const;

    /// Returns one of OBO::NoSignedWrap or OBO::NoUnsignedWrap.
    unsigned getNoWrapKind() const;
  };

  /// Common base class for all memory intrinsics. Simply provides
  /// common methods.
  /// Written as CRTP to avoid a common base class amongst the
  /// three atomicity hierarchies.
  template <typename Derived> class MemIntrinsicBase : public IntrinsicInst {
  private:
    enum { ARG_DEST = 0, ARG_LENGTH = 2 };

  public:
    Value *getRawDest() const {
      return const_cast<Value *>(getArgOperand(ARG_DEST));
    }
    const Use &getRawDestUse() const { return getArgOperandUse(ARG_DEST); }
    Use &getRawDestUse() { return getArgOperandUse(ARG_DEST); }

    Value *getLength() const {
      return const_cast<Value *>(getArgOperand(ARG_LENGTH));
    }
    const Use &getLengthUse() const { return getArgOperandUse(ARG_LENGTH); }
    Use &getLengthUse() { return getArgOperandUse(ARG_LENGTH); }

    /// This is just like getRawDest, but it strips off any cast
    /// instructions (including addrspacecast) that feed it, giving the
    /// original input.  The returned value is guaranteed to be a pointer.
    Value *getDest() const { return getRawDest()->stripPointerCasts(); }

    unsigned getDestAddressSpace() const {
      return cast<PointerType>(getRawDest()->getType())->getAddressSpace();
    }

    unsigned getDestAlignment() const { return getParamAlignment(ARG_DEST); }

    /// Set the specified arguments of the instruction.
    void setDest(Value *Ptr) {
      assert(getRawDest()->getType() == Ptr->getType() &&
             "setDest called with pointer of wrong type!");
      setArgOperand(ARG_DEST, Ptr);
    }

    void setDestAlignment(unsigned Align) {
      removeParamAttr(ARG_DEST, Attribute::Alignment);
      if (Align > 0)
        addParamAttr(ARG_DEST,
                     Attribute::getWithAlignment(getContext(), Align));
    }

    void setLength(Value *L) {
      assert(getLength()->getType() == L->getType() &&
             "setLength called with value of wrong type!");
      setArgOperand(ARG_LENGTH, L);
    }
  };

  /// Common base class for all memory transfer intrinsics. Simply provides
  /// common methods.
  template <class BaseCL> class MemTransferBase : public BaseCL {
  private:
    enum { ARG_SOURCE = 1 };

  public:
    /// Return the arguments to the instruction.
    Value *getRawSource() const {
      return const_cast<Value *>(BaseCL::getArgOperand(ARG_SOURCE));
    }
    const Use &getRawSourceUse() const {
      return BaseCL::getArgOperandUse(ARG_SOURCE);
    }
    Use &getRawSourceUse() { return BaseCL::getArgOperandUse(ARG_SOURCE); }

    /// This is just like getRawSource, but it strips off any cast
    /// instructions that feed it, giving the original input.  The returned
    /// value is guaranteed to be a pointer.
    Value *getSource() const { return getRawSource()->stripPointerCasts(); }

    unsigned getSourceAddressSpace() const {
      return cast<PointerType>(getRawSource()->getType())->getAddressSpace();
    }

    unsigned getSourceAlignment() const {
      return BaseCL::getParamAlignment(ARG_SOURCE);
    }

    void setSource(Value *Ptr) {
      assert(getRawSource()->getType() == Ptr->getType() &&
             "setSource called with pointer of wrong type!");
      BaseCL::setArgOperand(ARG_SOURCE, Ptr);
    }

    void setSourceAlignment(unsigned Align) {
      BaseCL::removeParamAttr(ARG_SOURCE, Attribute::Alignment);
      if (Align > 0)
        BaseCL::addParamAttr(ARG_SOURCE, Attribute::getWithAlignment(
                                             BaseCL::getContext(), Align));
    }
  };

  /// Common base class for all memset intrinsics. Simply provides
  /// common methods.
  template <class BaseCL> class MemSetBase : public BaseCL {
  private:
    enum { ARG_VALUE = 1 };

  public:
    Value *getValue() const {
      return const_cast<Value *>(BaseCL::getArgOperand(ARG_VALUE));
    }
    const Use &getValueUse() const {
      return BaseCL::getArgOperandUse(ARG_VALUE);
    }
    Use &getValueUse() { return BaseCL::getArgOperandUse(ARG_VALUE); }

    void setValue(Value *Val) {
      assert(getValue()->getType() == Val->getType() &&
             "setValue called with value of wrong type!");
      BaseCL::setArgOperand(ARG_VALUE, Val);
    }
  };

  // The common base class for the atomic memset/memmove/memcpy intrinsics
  // i.e. llvm.element.unordered.atomic.memset/memcpy/memmove
  class AtomicMemIntrinsic : public MemIntrinsicBase<AtomicMemIntrinsic> {
  private:
    enum { ARG_ELEMENTSIZE = 3 };

  public:
    Value *getRawElementSizeInBytes() const {
      return const_cast<Value *>(getArgOperand(ARG_ELEMENTSIZE));
    }

    ConstantInt *getElementSizeInBytesCst() const {
      return cast<ConstantInt>(getRawElementSizeInBytes());
    }

    uint32_t getElementSizeInBytes() const {
      return getElementSizeInBytesCst()->getZExtValue();
    }

    void setElementSizeInBytes(Constant *V) {
      assert(V->getType() == Type::getInt8Ty(getContext()) &&
             "setElementSizeInBytes called with value of wrong type!");
      setArgOperand(ARG_ELEMENTSIZE, V);
    }

    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memcpy_element_unordered_atomic:
      case Intrinsic::memmove_element_unordered_atomic:
      case Intrinsic::memset_element_unordered_atomic:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class represents atomic memset intrinsic
  // i.e. llvm.element.unordered.atomic.memset
  class AtomicMemSetInst : public MemSetBase<AtomicMemIntrinsic> {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::memset_element_unordered_atomic;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  // This class wraps the atomic memcpy/memmove intrinsics
  // i.e. llvm.element.unordered.atomic.memcpy/memmove
  class AtomicMemTransferInst : public MemTransferBase<AtomicMemIntrinsic> {
  public:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memcpy_element_unordered_atomic:
      case Intrinsic::memmove_element_unordered_atomic:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class represents the atomic memcpy intrinsic
  /// i.e. llvm.element.unordered.atomic.memcpy
  class AtomicMemCpyInst : public AtomicMemTransferInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::memcpy_element_unordered_atomic;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class represents the atomic memmove intrinsic
  /// i.e. llvm.element.unordered.atomic.memmove
  class AtomicMemMoveInst : public AtomicMemTransferInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::memmove_element_unordered_atomic;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This is the common base class for memset/memcpy/memmove.
  class MemIntrinsic : public MemIntrinsicBase<MemIntrinsic> {
  private:
    enum { ARG_VOLATILE = 3 };

  public:
    ConstantInt *getVolatileCst() const {
      return cast<ConstantInt>(
          const_cast<Value *>(getArgOperand(ARG_VOLATILE)));
    }

    bool isVolatile() const {
      return !getVolatileCst()->isZero();
    }

    void setVolatile(Constant *V) { setArgOperand(ARG_VOLATILE, V); }

    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memcpy:
      case Intrinsic::memmove:
      case Intrinsic::memset:
        return true;
      default: return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class wraps the llvm.memset intrinsic.
  class MemSetInst : public MemSetBase<MemIntrinsic> {
  public:
    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::memset;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class wraps the llvm.memcpy/memmove intrinsics.
  class MemTransferInst : public MemTransferBase<MemIntrinsic> {
  public:
    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::memcpy ||
             I->getIntrinsicID() == Intrinsic::memmove;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class wraps the llvm.memcpy intrinsic.
  class MemCpyInst : public MemTransferInst {
  public:
    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::memcpy;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class wraps the llvm.memmove intrinsic.
  class MemMoveInst : public MemTransferInst {
  public:
    // Methods for support type inquiry through isa, cast, and dyn_cast:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::memmove;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  // The common base class for any memset/memmove/memcpy intrinsics;
  // whether they be atomic or non-atomic.
  // i.e. llvm.element.unordered.atomic.memset/memcpy/memmove
  //  and llvm.memset/memcpy/memmove
  class AnyMemIntrinsic : public MemIntrinsicBase<AnyMemIntrinsic> {
  public:
    bool isVolatile() const {
      // Only the non-atomic intrinsics can be volatile
      if (auto *MI = dyn_cast<MemIntrinsic>(this))
        return MI->isVolatile();
      return false;
    }

    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memcpy:
      case Intrinsic::memmove:
      case Intrinsic::memset:
      case Intrinsic::memcpy_element_unordered_atomic:
      case Intrinsic::memmove_element_unordered_atomic:
      case Intrinsic::memset_element_unordered_atomic:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class represents any memset intrinsic
  // i.e. llvm.element.unordered.atomic.memset
  // and  llvm.memset
  class AnyMemSetInst : public MemSetBase<AnyMemIntrinsic> {
  public:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memset:
      case Intrinsic::memset_element_unordered_atomic:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  // This class wraps any memcpy/memmove intrinsics
  // i.e. llvm.element.unordered.atomic.memcpy/memmove
  // and  llvm.memcpy/memmove
  class AnyMemTransferInst : public MemTransferBase<AnyMemIntrinsic> {
  public:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memcpy:
      case Intrinsic::memmove:
      case Intrinsic::memcpy_element_unordered_atomic:
      case Intrinsic::memmove_element_unordered_atomic:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class represents any memcpy intrinsic
  /// i.e. llvm.element.unordered.atomic.memcpy
  ///  and llvm.memcpy
  class AnyMemCpyInst : public AnyMemTransferInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memcpy:
      case Intrinsic::memcpy_element_unordered_atomic:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This class represents any memmove intrinsic
  /// i.e. llvm.element.unordered.atomic.memmove
  ///  and llvm.memmove
  class AnyMemMoveInst : public AnyMemTransferInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      switch (I->getIntrinsicID()) {
      case Intrinsic::memmove:
      case Intrinsic::memmove_element_unordered_atomic:
        return true;
      default:
        return false;
      }
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This represents the llvm.va_start intrinsic.
  class VAStartInst : public IntrinsicInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::vastart;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }

    Value *getArgList() const { return const_cast<Value*>(getArgOperand(0)); }
  };

  /// This represents the llvm.va_end intrinsic.
  class VAEndInst : public IntrinsicInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::vaend;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }

    Value *getArgList() const { return const_cast<Value*>(getArgOperand(0)); }
  };

  /// This represents the llvm.va_copy intrinsic.
  class VACopyInst : public IntrinsicInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::vacopy;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }

    Value *getDest() const { return const_cast<Value*>(getArgOperand(0)); }
    Value *getSrc() const { return const_cast<Value*>(getArgOperand(1)); }
  };

  /// This represents the llvm.instrprof_increment intrinsic.
  class InstrProfIncrementInst : public IntrinsicInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::instrprof_increment;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }

    GlobalVariable *getName() const {
      return cast<GlobalVariable>(
          const_cast<Value *>(getArgOperand(0))->stripPointerCasts());
    }

    ConstantInt *getHash() const {
      return cast<ConstantInt>(const_cast<Value *>(getArgOperand(1)));
    }

    ConstantInt *getNumCounters() const {
      return cast<ConstantInt>(const_cast<Value *>(getArgOperand(2)));
    }

    ConstantInt *getIndex() const {
      return cast<ConstantInt>(const_cast<Value *>(getArgOperand(3)));
    }

    Value *getStep() const;
  };

  class InstrProfIncrementInstStep : public InstrProfIncrementInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::instrprof_increment_step;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }
  };

  /// This represents the llvm.instrprof_value_profile intrinsic.
  class InstrProfValueProfileInst : public IntrinsicInst {
  public:
    static bool classof(const IntrinsicInst *I) {
      return I->getIntrinsicID() == Intrinsic::instrprof_value_profile;
    }
    static bool classof(const Value *V) {
      return isa<IntrinsicInst>(V) && classof(cast<IntrinsicInst>(V));
    }

    GlobalVariable *getName() const {
      return cast<GlobalVariable>(
          const_cast<Value *>(getArgOperand(0))->stripPointerCasts());
    }

    ConstantInt *getHash() const {
      return cast<ConstantInt>(const_cast<Value *>(getArgOperand(1)));
    }

    Value *getTargetValue() const {
      return cast<Value>(const_cast<Value *>(getArgOperand(2)));
    }

    ConstantInt *getValueKind() const {
      return cast<ConstantInt>(const_cast<Value *>(getArgOperand(3)));
    }

    // Returns the value site index.
    ConstantInt *getIndex() const {
      return cast<ConstantInt>(const_cast<Value *>(getArgOperand(4)));
    }
  };

} // end namespace llvm

#endif // LLVM_IR_INTRINSICINST_H
