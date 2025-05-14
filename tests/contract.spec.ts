const { Blockchain, nameToBigInt } = require("@eosnetwork/vert");
const { assert } = require("chai");
const blockchain = new Blockchain()
const { Asset } = require("@greymass/eosio");
const symSALISH = Asset.SymbolCode.from('SALISH')

// Load contract (use paths relative to the root of the project)
const validator = blockchain.createContract('validator', 'build/validator')

/* Runs before each test */
beforeEach(async () => {
    blockchain.resetTables()
})

/* Tests */
describe('Test', () => {
    it('testing', async () => {
        const [usera, userb] = blockchain.createAccounts('usera', 'userb')
        const result = await validator.actions.validate(['usera', 'userb', '10.00 COSEEDS', 'moomoo']).send('usera@active')
        await validator.actions.addproduct(['usera', 'meat']).send('usera@active');
        await validator.actions.addproduct(['usera', 'potatoes']).send('usera@active');
        await validator.actions.addterm(['usera', 'meat', 'pork']).send('usera@active');
        await validator.actions.addterm(['usera', 'meat', 'beef']).send('usera@active');
        console.log(validator.tables.products([nameToBigInt('validator')]).getTableRows());        

        await validator.actions.addgap(['usera', symSALISH, 'tokens4biofi', 'meat']).send('usera@active');
        console.log(validator.tables.gaps([nameToBigInt('validator')]).getTableRows());        
    });
});
