class OwnedThing {
  OwnedThing();
};

virtual class VirtualBase {
};

virtual class VirtualDerived : VirtualBase {
  VirtualDerived();
};

namespace wraptest {

virtual class NamespacedVirtualBase {
};

virtual class NamespacedVirtualDerived : NamespacedVirtualBase {
  NamespacedVirtualDerived();
};

}  // namespace wraptest
