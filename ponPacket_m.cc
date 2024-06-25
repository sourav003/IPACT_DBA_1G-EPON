//
// Generated file, do not edit! Created by opp_msgtool 6.0 from ponPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "ponPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(ponPacket)

ponPacket::ponPacket(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

ponPacket::ponPacket(const ponPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

ponPacket::~ponPacket()
{
}

ponPacket& ponPacket::operator=(const ponPacket& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void ponPacket::copy(const ponPacket& other)
{
    this->GenerationTime = other.GenerationTime;
    this->OnuArrivalTime = other.OnuArrivalTime;
    this->OnuDepartureTime = other.OnuDepartureTime;
    this->OnuId = other.OnuId;
    this->IsGrant = other.IsGrant;
    this->Grant = other.Grant;
    this->IsRequest = other.IsRequest;
    this->Request = other.Request;
}

void ponPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->GenerationTime);
    doParsimPacking(b,this->OnuArrivalTime);
    doParsimPacking(b,this->OnuDepartureTime);
    doParsimPacking(b,this->OnuId);
    doParsimPacking(b,this->IsGrant);
    doParsimPacking(b,this->Grant);
    doParsimPacking(b,this->IsRequest);
    doParsimPacking(b,this->Request);
}

void ponPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->GenerationTime);
    doParsimUnpacking(b,this->OnuArrivalTime);
    doParsimUnpacking(b,this->OnuDepartureTime);
    doParsimUnpacking(b,this->OnuId);
    doParsimUnpacking(b,this->IsGrant);
    doParsimUnpacking(b,this->Grant);
    doParsimUnpacking(b,this->IsRequest);
    doParsimUnpacking(b,this->Request);
}

omnetpp::simtime_t ponPacket::getGenerationTime() const
{
    return this->GenerationTime;
}

void ponPacket::setGenerationTime(omnetpp::simtime_t GenerationTime)
{
    this->GenerationTime = GenerationTime;
}

omnetpp::simtime_t ponPacket::getOnuArrivalTime() const
{
    return this->OnuArrivalTime;
}

void ponPacket::setOnuArrivalTime(omnetpp::simtime_t OnuArrivalTime)
{
    this->OnuArrivalTime = OnuArrivalTime;
}

omnetpp::simtime_t ponPacket::getOnuDepartureTime() const
{
    return this->OnuDepartureTime;
}

void ponPacket::setOnuDepartureTime(omnetpp::simtime_t OnuDepartureTime)
{
    this->OnuDepartureTime = OnuDepartureTime;
}

int ponPacket::getOnuId() const
{
    return this->OnuId;
}

void ponPacket::setOnuId(int OnuId)
{
    this->OnuId = OnuId;
}

bool ponPacket::getIsGrant() const
{
    return this->IsGrant;
}

void ponPacket::setIsGrant(bool IsGrant)
{
    this->IsGrant = IsGrant;
}

double ponPacket::getGrant() const
{
    return this->Grant;
}

void ponPacket::setGrant(double Grant)
{
    this->Grant = Grant;
}

bool ponPacket::getIsRequest() const
{
    return this->IsRequest;
}

void ponPacket::setIsRequest(bool IsRequest)
{
    this->IsRequest = IsRequest;
}

double ponPacket::getRequest() const
{
    return this->Request;
}

void ponPacket::setRequest(double Request)
{
    this->Request = Request;
}

class ponPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_GenerationTime,
        FIELD_OnuArrivalTime,
        FIELD_OnuDepartureTime,
        FIELD_OnuId,
        FIELD_IsGrant,
        FIELD_Grant,
        FIELD_IsRequest,
        FIELD_Request,
    };
  public:
    ponPacketDescriptor();
    virtual ~ponPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ponPacketDescriptor)

ponPacketDescriptor::ponPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(ponPacket)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

ponPacketDescriptor::~ponPacketDescriptor()
{
    delete[] propertyNames;
}

bool ponPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ponPacket *>(obj)!=nullptr;
}

const char **ponPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ponPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ponPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 8+base->getFieldCount() : 8;
}

unsigned int ponPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_GenerationTime
        FD_ISEDITABLE,    // FIELD_OnuArrivalTime
        FD_ISEDITABLE,    // FIELD_OnuDepartureTime
        FD_ISEDITABLE,    // FIELD_OnuId
        FD_ISEDITABLE,    // FIELD_IsGrant
        FD_ISEDITABLE,    // FIELD_Grant
        FD_ISEDITABLE,    // FIELD_IsRequest
        FD_ISEDITABLE,    // FIELD_Request
    };
    return (field >= 0 && field < 8) ? fieldTypeFlags[field] : 0;
}

const char *ponPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "GenerationTime",
        "OnuArrivalTime",
        "OnuDepartureTime",
        "OnuId",
        "IsGrant",
        "Grant",
        "IsRequest",
        "Request",
    };
    return (field >= 0 && field < 8) ? fieldNames[field] : nullptr;
}

int ponPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "GenerationTime") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "OnuArrivalTime") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "OnuDepartureTime") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "OnuId") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "IsGrant") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "Grant") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "IsRequest") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "Request") == 0) return baseIndex + 7;
    return base ? base->findField(fieldName) : -1;
}

const char *ponPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "omnetpp::simtime_t",    // FIELD_GenerationTime
        "omnetpp::simtime_t",    // FIELD_OnuArrivalTime
        "omnetpp::simtime_t",    // FIELD_OnuDepartureTime
        "int",    // FIELD_OnuId
        "bool",    // FIELD_IsGrant
        "double",    // FIELD_Grant
        "bool",    // FIELD_IsRequest
        "double",    // FIELD_Request
    };
    return (field >= 0 && field < 8) ? fieldTypeStrings[field] : nullptr;
}

const char **ponPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ponPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ponPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ponPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ponPacket'", field);
    }
}

const char *ponPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ponPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        case FIELD_GenerationTime: return simtime2string(pp->getGenerationTime());
        case FIELD_OnuArrivalTime: return simtime2string(pp->getOnuArrivalTime());
        case FIELD_OnuDepartureTime: return simtime2string(pp->getOnuDepartureTime());
        case FIELD_OnuId: return long2string(pp->getOnuId());
        case FIELD_IsGrant: return bool2string(pp->getIsGrant());
        case FIELD_Grant: return double2string(pp->getGrant());
        case FIELD_IsRequest: return bool2string(pp->getIsRequest());
        case FIELD_Request: return double2string(pp->getRequest());
        default: return "";
    }
}

void ponPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        case FIELD_GenerationTime: pp->setGenerationTime(string2simtime(value)); break;
        case FIELD_OnuArrivalTime: pp->setOnuArrivalTime(string2simtime(value)); break;
        case FIELD_OnuDepartureTime: pp->setOnuDepartureTime(string2simtime(value)); break;
        case FIELD_OnuId: pp->setOnuId(string2long(value)); break;
        case FIELD_IsGrant: pp->setIsGrant(string2bool(value)); break;
        case FIELD_Grant: pp->setGrant(string2double(value)); break;
        case FIELD_IsRequest: pp->setIsRequest(string2bool(value)); break;
        case FIELD_Request: pp->setRequest(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ponPacket'", field);
    }
}

omnetpp::cValue ponPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        case FIELD_GenerationTime: return pp->getGenerationTime().dbl();
        case FIELD_OnuArrivalTime: return pp->getOnuArrivalTime().dbl();
        case FIELD_OnuDepartureTime: return pp->getOnuDepartureTime().dbl();
        case FIELD_OnuId: return pp->getOnuId();
        case FIELD_IsGrant: return pp->getIsGrant();
        case FIELD_Grant: return pp->getGrant();
        case FIELD_IsRequest: return pp->getIsRequest();
        case FIELD_Request: return pp->getRequest();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ponPacket' as cValue -- field index out of range?", field);
    }
}

void ponPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        case FIELD_GenerationTime: pp->setGenerationTime(value.doubleValue()); break;
        case FIELD_OnuArrivalTime: pp->setOnuArrivalTime(value.doubleValue()); break;
        case FIELD_OnuDepartureTime: pp->setOnuDepartureTime(value.doubleValue()); break;
        case FIELD_OnuId: pp->setOnuId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_IsGrant: pp->setIsGrant(value.boolValue()); break;
        case FIELD_Grant: pp->setGrant(value.doubleValue()); break;
        case FIELD_IsRequest: pp->setIsRequest(value.boolValue()); break;
        case FIELD_Request: pp->setRequest(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ponPacket'", field);
    }
}

const char *ponPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ponPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ponPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ponPacket *pp = omnetpp::fromAnyPtr<ponPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ponPacket'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

