#pragma once
#ifndef IPSTRUCTS_H
#define IPSTRUCTS_H

#include <memory>

#include "IPv4address.h"
#include "IPv4mask.h"

namespace core {
    class NetworkBase
    {
    public:
        std::shared_ptr<IPaddressBase> Ip;
        std::shared_ptr<IPmaskBase> NetMask;

        bool isHost(const IPaddressBase& hostIP) const
        {
            return ( (*(*Ip & *NetMask) == *(hostIP & *NetMask)) && (hostIP != *Ip) ) ? true : false;
        };
        virtual unsigned long long hostsCapacity() const
        {
            auto allAddresses = 1ull << (NetMask->getLength() - NetMask->getPrefix());
            return allAddresses - 1; //without network address
        }
        void fix()
        {
            Ip = *Ip & *NetMask;
        };

        std::shared_ptr<NetworkBase> clone() const
        {
            return std::shared_ptr<NetworkBase>(_cloneImpl());
        }

        virtual ~NetworkBase() = default;
    protected:
        NetworkBase() = default;
        NetworkBase(const NetworkBase&) = default;
        NetworkBase& operator=(const NetworkBase&) = default;
        NetworkBase(NetworkBase&&) noexcept = default;
        NetworkBase& operator=(NetworkBase&&) noexcept = default;
    private:
        virtual NetworkBase* _cloneImpl() const = 0;
    };

    class Subnet: public NetworkBase
    {
    public:
        long long int HostNumber = -10;
        QString SubName = "blank";

        virtual std::unique_ptr<IPaddressBase> getMinHost() = 0;
        virtual std::unique_ptr<IPaddressBase> getMaxHost() = 0;
        virtual std::unique_ptr<IPaddressBase> getBroadcast() = 0;

        std::shared_ptr<Subnet> clone() const
        {
            return std::shared_ptr<Subnet>(_cloneImpl());
        }
    protected:
        Subnet() = default;
        Subnet(const Subnet&) = default;
        Subnet& operator=(const Subnet&) = default;
        Subnet(Subnet&&) noexcept = default;
        Subnet& operator=(Subnet&&) noexcept = default;
    private:
        Subnet* _cloneImpl() const override = 0;
    };

    class Networkv4: public NetworkBase
    {
    public:
        Networkv4() { Ip = std::make_shared<IPv4address>(); NetMask = std::make_shared<IPv4mask>(); };
        virtual unsigned long long hostsCapacity() const override
        {
            return NetworkBase::hostsCapacity() - 1; //without broadcast
        }
    private:
        virtual Networkv4* _cloneImpl() const override
        {
            auto ptr = new Networkv4;
            *ptr->Ip = *this->Ip;
            *ptr->NetMask = *this->NetMask;
            return ptr;
        }
    };

    class Subnetv4: public Subnet
    {
    public:
        Subnetv4() { Ip = std::make_shared<IPv4address>(); NetMask = std::make_shared<IPv4mask>(); };
        virtual unsigned long long hostsCapacity() const override
        {
            return NetworkBase::hostsCapacity() - 1; //without broadcast
        }
        std::unique_ptr<IPaddressBase> getMinHost() override
        {
            auto x = boost::dynamic_bitset<>(32, 1);
            return std::make_unique<IPv4address>(dynamic_cast<IPv4address&>(*Ip) | IPv4address{x});
        }
        std::unique_ptr<IPaddressBase> getMaxHost() override
        {
            auto x = boost::dynamic_bitset<>(32);
            x.set(1, 32 - NetMask->getPrefix() - 1, true);
            return std::make_unique<IPv4address>(dynamic_cast<IPv4address&>(*Ip) | IPv4address{x});
        }
        std::unique_ptr<IPaddressBase> getBroadcast() override
        {
            auto x = boost::dynamic_bitset<>(32);
            x.set(0, 32 - NetMask->getPrefix(), true);
            return std::make_unique<IPv4address>(dynamic_cast<IPv4address&>(*Ip) | IPv4address{x});
        }
    private:
        virtual Subnetv4* _cloneImpl() const override
        {
            auto ptr = new Subnetv4;
            *ptr->Ip = *this->Ip;
            *ptr->NetMask = *this->NetMask;
            ptr->HostNumber = this->HostNumber;
            ptr->SubName = this->SubName;
            return ptr;
        }
    };
};

#endif // IPSTRUCTS_H
