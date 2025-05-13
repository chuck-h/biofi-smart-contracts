#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

//contractName:validator

using namespace eosio;

CONTRACT validator : public contract {
    public:
        using contract::contract;

        // Every ACTION you define can be called from outside the blockchain
        ACTION validate( name from, name to, asset quantity, std::string memo ){
            check(false, "hello, joe");
        }
};
