namespace scalar_contracts
{

    class ScalarContract
    {
        ScalarContract();
        void setStringConstRef(const string &value);
        void setStringValue(string value);
        string getStringValue() const;
        uint8_t echoUint8(uint8_t value) const;
        int8_t echoInt8(int8_t value) const;
        uint16_t echoUint16(uint16_t value) const;
        int16_t echoInt16(int16_t value) const;
        uint32_t echoUint32(uint32_t value) const;
        int32_t echoInt32(int32_t value) const;
        uint64_t echoUint64(uint64_t value) const;
        int64_t echoInt64(int64_t value) const;
        std::uint32_t echoStdUint32(std::uint32_t value) const;
    };

    uint32_t GlobalUint32(uint32_t value);
    void GlobalStringConstRef(const string &value);

} // namespace scalar_contracts
