#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

//contractName:validator

using namespace eosio;
using std::string;


      

      
TABLE account { // scoped on account name
   asset    balance;
   uint64_t primary_key()const { return balance.symbol.code().raw(); }
};



        
typedef eosio::multi_index< "accounts"_n, account > accounts;         

CONTRACT validator : public contract {
    public:
        using contract::contract;
        
TABLE product { // single table, scoped by contract account name
   name     product;
   std::vector<string>   terms;
   uint64_t primary_key() const { return product.value; }
};
typedef eosio::multi_index< "products"_n, product > products;                 

TABLE gap { // single table, scoped by contract account name
   uint64_t     id;
   symbol_code  sym_code;
   name         contract;
   name         product;
   uint64_t primary_key() const { return id; }
   uint64_t secondary_key() const { return sym_code.raw(); }
};

typedef eosio::multi_index< "gaps"_n, gap, indexed_by
               < "bysymcode"_n,
                 const_mem_fun<gap, uint64_t, &gap::secondary_key >
               >
             > gaps;

        ACTION validate( name from, name to, asset quantity, std::string memo ){
            check(memo.length()>0, "product/service must appear in memo");
            /* TODO
               tokenize memo
               extract sym_code from quantity
               iterate over gaps table (indexed by symcode) elements matching sym_code
               for each gap entry (check symcode & contract are right),
                 look up terms vector in product table
                 if any memo token appears in terms vector, error
                 "your community cannot provide this"
            */
            
        }
        
        ACTION addproduct( name actor, name product) {
          require_auth(actor);
          products prods(get_self(), get_self().value);
          prods.emplace(actor, [&]( auto& s ) {
            s.product = product;
            s.terms = std::vector<string>();
         });
       }
        
        ACTION addterm( name actor, name product, string term) {
          require_auth(actor);
          products prods(get_self(), get_self().value);
          auto p = prods.require_find(product.value, (product.to_string()+": unknown product").c_str());
          std::vector<string> t = p->terms;
          t.push_back(term);
          prods.modify(p, actor, [&]( auto& s ) {
            s.terms = t;
         });
       }

        ACTION addgap( name actor, symbol_code sym, name contract, name product) {
          require_auth(actor);
          gaps gp(get_self(), get_self().value);
          gp.emplace(actor, [&]( auto& s ) {
            s.id = gp.available_primary_key();
            s.sym_code = sym;
            s.contract = contract;
            s.product = product;
         });
       }

           /*

        ACTION remove_term()
        ACTION remove_product()
        ACTION add_gap()
        ACTION remove_gap()
        */
};
