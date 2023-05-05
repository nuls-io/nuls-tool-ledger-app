## General Transaction Date Protocol

### General-Transaction

[https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/Transaction.java#L131](https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/Transaction.java#L131)

|**Field**|**Size (bytes)**|**Description**|
|:----|:----|:----|
| type                     |      2       | little-endian，uint16，transaction type            |
| time                     |      4       | little-endian，uint32，transaction creation time        |
| remark_len               |     1-9      | Varint，length                    |
| remark                   |     variable      | remakr，utf8 byte array                  |
| txData_len               |     1-9      | varint, length of transaction data            |
| txData                   |     variable      | transaction data                        |
| coinData_len             |     1-9      | varint, length of transaction asset data            |
| coinData                 |     variable      | date of transaction assets as the following :General-CoinData |


#### General-CoinData

[https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/CoinData.java#L88](https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/CoinData.java#L88v)

|**Field**|**Size (bytes)**|**Description**|
|:----|:----|:----|
|from_count|1|the number of “from” is counted by the next required iteration|
| from     |     variable      | CoinFrom, Transaction sender, see the table below: General-CoinFrom" |
|to_count|1|the number of “to” is counted by the next required iteration|
| to       |     variable      | CoinTo, transaction receiver, see the table below: General-CoinTo     |


#### General-CoinFrom

[https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/CoinFrom.java#L62](https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/CoinFrom.java#L62)

|**Field**|**Size (bytes)**|**Description**|
|:----|:----|:----|
| address_len   |     1-9      | varint，address length                                |
| address       |     variable      | sender address                                        |
| assetsChainId |      2       | little-endian，uint16，AssetChainID                        |
| assetsId      |      2       | little-endian，uint16，asset ID                          |
| amount        |      32      | little-endian，transaction amount                                |
| nonce_len     |     1-9      | varint，length of transaction serial number                          |
| nonce         |     variable      | transaction serial number，prevent doubled transaction                        |
| locked        |      1       | 0normal transaction，-1unlock assests transaction（exit consensus，undelegate） |


#### General-CoinTO

[https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/CoinTo.java#L48](https://github.com/nuls-io/nuls-v2/blob/master/common/nuls-base/src/main/java/io/nuls/base/data/CoinTo.java#L48)

|**Field**|**Size (bytes)**|**Description**|
|:----|:----|:----|
| address_len   |     1-9      | varint，address length                      |
| address       |     variable      | receiving address                              |
| assetsChainId |      2       | little-endian，uint16，assetsChainId              |
| assetsId      |      2       | little-endian，uint16，assetsId                |
| amount        |      32      | little-endian，transaction amount                      |
| lockTime      |      8       | little-endian，int64，time of unlocking，-1 is permanent lock |


## Transaction Date-txData

### 2-Transfer-transfer

Date structure shown in general transaction

#### Display Items

|Title|Value|Description|
|:----|:----|:----|
|Type|Transfer|transaction type|
|Amount|NULS 8.88 / 9-55 898989|amount（refer to the "Note" below）|
|Address|NULSd....|receiver address|
|Fee|NULS 0.088|transaction fee|

Note: When the transferred asset is an internal supported asset, the number shown is the de-precisioned value. For example, if transferring the NULS asset with AssetId=1-1, and the parsed value is 135000000, with the NULS asset having a precision of 8, then the amount displayed on the Ledger is 1.35 NULS, which equals 135000000 / 1e8

#### Transaction Demonstration

```json
testing feedback message: e006000096058000002c8000003c8000000000000000000000000200d0f9fe6300008c0117010001b2a74c73b3d63ceae361f05e00355115d0255d3f01000100005d880600000000000000000000000000000000000000000000000000000000080000000000000000000117010001aca553215d606c663cac99e8503abe19f76289eb0100010060d68606000000000000000000000000000000000000000000000000000000000000000000000000
```

```json
{
  "hash": "bbc1306923568c72690b5ff6f95bfcbffc811c1490b61237720251f879ae37db",
  "type": 2,
  "timestamp": 1677654480,
  "remark": null,
  "transactionSignature": "2102b638c69aebc4d2b952aff586e658023a7ba5abfac065c7ff7175c6309b5a030946304402201ce0c4e459a55ffb2cf7b5cc2055d1510d1ec5f435c2b028b55d434391a8780802200a1707dce78a089c3fdd8d0287041e57cb8fbd3a9934ba77a0f38bfb5a674360",
  "txDataHex": null,
  "from": [
    {
      "address": "NULSd6HgerWoEd1c7LFtgSLfpPVkddL5eKrca",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "109600000",
      "nonce": "0000000000000000",
      "locked": 0
    }
  ],
  "to": [
    {
      "address": "NULSd6HgeV64HLRvTP1FacTcpCchdLUH6VVti",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "109500000",
      "lockTime": 0
    }
  ]
}
```

### 5-Join Consensus-stake

[https://github.com/nuls-io/nuls-v2/blob/master/module/nuls-consensus/nuls-consensus-poc/src/main/java/io/nuls/poc/model/bo/tx/txdata/Deposit.java#L83](https://github.com/nuls-io/nuls-v2/blob/master/module/nuls-consensus/nuls-consensus-poc/src/main/java/io/nuls/poc/model/bo/tx/txdata/Deposit.java#L83)

|**Field**|**Size (bytes)**|**Description**|
|:----|:----|:----|
| deposit   |      32      | little-endian，amount staked |
| address   |      23      | user address         |
| agentHash |      32      | hash of staking nodes     |


#### Display Items

|Title|Value|Description|
|:----|:----|:----|
|Type|Depoist|transaction type|
|Amount|NULS 8888.88|amount staked（refer to the "Note" below）|
|Fee|NULS 0.088|transaction fee|

Note: The asset of this transaction has to be NULS, and the amount displayed is equal to deposit / 1e8.

#### Transaction Demonstration

```json
testing feedback message: e006000096058000002c8000003c80000000000000000000000005003963f9630057a0f09cf1a80000000000000000000000000000000000000000000000000000000100012640fb7483f42e7bd467a556770e2ac3914483fa423735d1451bcab64122b3145d969c334f78ddec9be4db2e2e3abffb413f8c3e8c01170100012640fb7483f42e7bd467a556770e2ac3914483fa0100010040779ef1a80000000000000000000000000000000000000000000000000000000873f19c9399965cd50001170100012640fb7483f42e7bd467a556770e2ac3914483fa01000100a0f09cf1a8000000000000000000000000000000000000000000000000000000ffffffffffffffff
```

```json
{
  "hash": "e5ac585e6c61fe286359e89e54fdf14069a7f8935469cbf4201dec5800e9dabc",
  "type": 5,
  "timestamp": 1677288249,
  "remark": null,
  "transactionSignature": "21039451999ef23d53fc30ac730cc55a1352d65b50fa2fd2d43891021f96dc1fe386473045022100af10603498e8faa6d631e0d06c0debf8c82e40edeabf51857612524a25b36f4b02203a9d2288d40fec9ccf8d9d0d512b3dce2965420b313cdb88f3a3aa03a84f5a40",
  "txDataHex": "a0f09cf1a80000000000000000000000000000000000000000000000000000000100012640fb7483f42e7bd467a556770e2ac3914483fa423735d1451bcab64122b3145d969c334f78ddec9be4db2e2e3abffb413f8c3e",
  "from": [
    {
      "address": "NULSd6HgWDn4NoXR5d9m18SK9PvefK3fs9EUe",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "725608200000",
      "nonce": "73f19c9399965cd5",
      "locked": 0
    }
  ],
  "to": [
    {
      "address": "NULSd6HgWDn4NoXR5d9m18SK9PvefK3fs9EUe",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "725608100000",
      "lockTime": -1
    }
  ],
  "txData": {
    "deposit": 725608100000,
    "agentHash": "423735d1451bcab64122b3145d969c334f78ddec9be4db2e2e3abffb413f8c3e",
    "address": "NULSd6HgWDn4NoXR5d9m18SK9PvefK3fs9EUe"
  }
}
```

### 6-leave consensus-unstake

[https://github.com/nuls-io/nuls-v2/blob/master/module/nuls-consensus/nuls-consensus-poc/src/main/java/io/nuls/poc/model/bo/tx/txdata/CancelDeposit.java#L87](https://github.com/nuls-io/nuls-v2/blob/master/module/nuls-consensus/nuls-consensus-poc/src/main/java/io/nuls/poc/model/bo/tx/txdata/CancelDeposit.java#L87)

|**Field**|**Size (bytes)**|**Description**|
|:----|:----|:----|
| joinTxHash |      32      | transaction hash of staking |


#### Display Items

|Title|Value|Description|
|:----|:----|:----|
|Type|Withdraw|transaction type|
|Amount|NULS 8888.88|amount of unstaking(Quit deposit, extract “amount” from "CoinTo")（refer to the "Note" below）|
|Fee|NULS 0.088|transaction fee|

Note: The asset of this transaction has to be NULS, and the amount displayed is equal to deposit / 1e8

#### Transaction Demonstration

```json
testing feedback message: e006000096058000002c8000003c8000000000000000000000000600bc77fe630020e5ac585e6c61fe286359e89e54fdf14069a7f8935469cbf4201dec5800e9dabc8c01170100012640fb7483f42e7bd467a556770e2ac3914483fa01000100a0f09cf1a800000000000000000000000000000000000000000000000000000008201dec5800e9dabcff01170100012640fb7483f42e7bd467a556770e2ac3914483fa01000100006a9bf1a80000000000000000000000000000000000000000000000000000000000000000000000
```

```json
{
  "hash": "6b6fcd4619d44232f376e40a45323b70a77b7811a559e5e93c7ffa8e4f9e410f",
  "type": 6,
  "timestamp": 1677621180,
  "remark": null,
  "transactionSignature": "21039451999ef23d53fc30ac730cc55a1352d65b50fa2fd2d43891021f96dc1fe38647304502210090edd5f40363713d6e402e2b21cf59ff3f3138f82c7ed4947d2c8f34292f8caa022064ffbae23f4841b1e50baff08397279db2e1126129d53d8e0760e3e5e6ef5db3",
  "txDataHex": "e5ac585e6c61fe286359e89e54fdf14069a7f8935469cbf4201dec5800e9dabc",
  "from": [
    {
      "address": "NULSd6HgWDn4NoXR5d9m18SK9PvefK3fs9EUe",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "725608100000",
      "nonce": "201dec5800e9dabc",
      "locked": -1
    }
  ],
  "to": [
    {
      "address": "NULSd6HgWDn4NoXR5d9m18SK9PvefK3fs9EUe",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "725608000000",
      "lockTime": 0
    }
  ],
  "txData": {
    "joinTxHash": "e5ac585e6c61fe286359e89e54fdf14069a7f8935469cbf4201dec5800e9dabc"
  }
}
```

### 10-Cross Chain Transfer-Crosschain Transaction

Date structure shown in general transaction

#### Display Items

|Title|Value|Description|
|:----|:----|:----|
|Type|Cross Chain Transfer|transaction type|
|Amount|NULS 8.888 / 9-55 898989|amount（refer to the "Note" below）|
|Address|NULSd.... / NERVEe....|receiver address(if the address prefix changes, please refer to "Note 2")|
|Fee|NULS 0.088|transaction fee|

Note: When transferring an internal asset, the amount displayed is the de-precisioned value. For example, if transferring the NULS asset with "AssetId=1-1", and the parsed value is 135000000, with the NULS asset having a precision of 8, then the amount displayed on the Ledger is 1.35 NULS, which equals 135000000 / 1e8

Note2：The address data is read as a uint16 (little-endian) and converted to decimal. If the value is 9, the address prefix used is NERVEe. If the value is 1, the address prefix used is NULSd

#### Transaction Demonstration（1）

Crosschain transaction with NULS assets

```json
testing feedback message: e006000096058000002c8000003c8000000000000000000000000a000321066400008c0117010001e6da0a93f2ac0f396745215115fe359ae3dafc2f01000100400caa3b0000000000000000000000000000000000000000000000000000000008536cb627f749afed000117090001e6da0a93f2ac0f396745215115fe359ae3dafc2f0100010000ca9a3b000000000000000000000000000000000000000000000000000000000000000000000000
```

```json
{
  "hash": "38e92a2db7d02cc305dc528a0dd4e8d61f9b9b007f2050945cc05de1b65bfbb5",
  "type": 10,
  "timestamp": 1678123267,
  "remark": null,
  "transactionSignature": "2102d1ca09495b5a251f72db575f80913b3fdc0df11868e9931643d3580265577ca3473045022100c4314dcb0f4395f40b0c1427aadf7e45d4659fdd89d16d38b24204c2ee088988022041e9a6d344908c484487c07f6ba60ae4a8806d584b13698a7df95176d9a97643",
  "txDataHex": null,
  "from": [
    {
      "address": "NULSd6Hgi4gPrtfxevMJ8BPdRi9dD6kh3WYgQ",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "1001000000",
      "nonce": "536cb627f749afed",
      "locked": 0
    }
  ],
  "to": [
    {
      "address": "NERVEepb6F5DjGnGB1eHpyssPogq7Y92uCznsz",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "1000000000",
      "lockTime": 0
    }
  ]
}
```

#### Transaction Demonstration（2）

Crosschain transaction with other assets

```json
testing feedback message: e006000096058000002c8000003c8000000000000000000000000a005d7906640000d20217010001e6da0a93f2ac0f396745215115fe359ae3dafc2f0100010040420f0000000000000000000000000000000000000000000000000000000000085cc05de1b65bfbb50017010001e6da0a93f2ac0f396745215115fe359ae3dafc2f09000e0296bdc3cadec2ae55cd2811e50000000000000000000000000000000000000000086da2a545a4405046000117090001e6da0a93f2ac0f396745215115fe359ae3dafc2f09000e0296bdc3cadec2ae55cd2811e500000000000000000000000000000000000000000000000000000000
```

```json
{
  "hash": "302808dbce3104c51a28c799ed946227b68098fd4a3d64aadd7a66f4b3baec2e",
  "type": 10,
  "timestamp": 1678145885,
  "remark": null,
  "transactionSignature": "2102d1ca09495b5a251f72db575f80913b3fdc0df11868e9931643d3580265577ca3473045022100a7bfe00124f38a4ca14f8f2ced58d29d69fa4b20b915d731df12e9496a27926402205f5cc753ca5208f717884eb57000bb8eda471e0660a22ddecc2ea5fd3803add7",
  "txDataHex": null,
  "from": [
    {
      "address": "NULSd6Hgi4gPrtfxevMJ8BPdRi9dD6kh3WYgQ",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "1000000",
      "nonce": "5cc05de1b65bfbb5",
      "locked": 0
    },
    {
      "address": "NULSd6Hgi4gPrtfxevMJ8BPdRi9dD6kh3WYgQ",
      "assetsChainId": 9,
      "assetsId": 526,
      "amount": "70892811670437405731523378582",
      "nonce": "6da2a545a4405046",
      "locked": 0
    }
  ],
  "to": [
    {
      "address": "NERVEepb6F5DjGnGB1eHpyssPogq7Y92uCznsz",
      "assetsChainId": 9,
      "assetsId": 526,
      "amount": "70892811670437405731523378582",
      "lockTime": 0
    }
  ]
}
```


### 16-call Contract-Call Contract

[https://github.com/nuls-io/nuls-v2/blob/master/module/nuls-smart-contract/src/main/java/io/nuls/contract/model/txdata/CallContractData.java#L108](https://github.com/nuls-io/nuls-v2/blob/master/module/nuls-smart-contract/src/main/java/io/nuls/contract/model/txdata/CallContractData.java#L108)

|**Field**|**Size (bytes)**|**Description**|
|:----|:----|:----|
| sender                   |      23      | caller address                                                   |
| contractAddress          |      23      | contract address                                                     |
| value                    |      32      | little-endian， the amount of NULS transferred from the caller to the contract address，fill with 0 if there is none      |
| gasLimit                 |      8       | little-endian，int64，max gas consumed                                   |
| gasPrice                 |      8       | little-endian，int64，gas price                                  |
| methodName_len           |     1-9      | Method name length                                                 |
| methodName               |     variable      | utf8 byte array，method name                                         |
| methodDesc_len           |     1-9      | Method describe length                                                 |
| methodDesc               |     variable      | utf8 byte array，method describe，if the contract method is not overloaded，this parameter can be empty |
| two_dimensional_args_len |      1       | length of a two-dimensional array，uint8，the next 3 need to be read cyclically                    |
| one_dimensional_args_len |      1       | length of a one-dimensional array，uint8，the next 2 need to be read cyclically                    |
| element_len              |     1-9      | byte length of each element in a one-dimensional array                                         |
| element                  |     variable      | UTF-8 byte array of each element in a one-dimensional array                                     |


#### Display Items (1)

|Title|Value|Description|
|:----|:----|:----|
|Type|Call Contract|call contract|
|Amount|NULS 8.88|the amount of NULS transferred to the contract by the caller，use txData.value|
|Contract Address|NULSd.....|contract address|
|Contract Method|swap...|method name|
|Fee|NULS 0.088|transaction fee|

#### Display Items (2)

|Title|Value|Description|
|:----|:----|:----|
|Type|NRC20 Transfer|NRC20 transfer|
|Amount|NABOX 8.88|amount（refer to the "Note" below）|
|Address|NULSd.....|receiving address|
|Fee|NULS 0.088|transaction fee|

Note: When the contract address is a address of an internal NRC20 asset contract, the contract method is `transfer`or`transferFrom` ，shown as `NRC20 transfer` ，and the number is displayed in a de-serialized format. For example，when transferring the NABOX asset with contract address （NULSd6Hgv3Y49CHapt5qL4pCiEX8x3ZHrh6ie）, the parsed contract parameter is value=3350000000000000000, and the precision of the NABOX asset is 18. Therefore, the amount displayed on Ledger is 3.35 NABOX, which is 3350000000000000000 / 1e18

`transfer` parsed into two parameters： "to" and "value", as shown in Transaction Example 1.

`transferFrom` parsed into two parameters：from, to, value.

#### Transaction Demonstration (1)

NRC20 transfer

```json
testing feedback message: e006000096058000002c8000003c80000000000000000000000010000a365060009901000155b03381577d329bb52dd47062afcb80576ebdf3010002a443687a6883154a1a94e8b66419c1490716d0b20000000000000000000000000000000000000000000000000000000000000000a0860100000000001900000000000000087472616e73666572000201254e554c53643648675a7068736e3151734d6d414b3573613247724a3279704d4c327852416301073130303030303048011701000155b03381577d329bb52dd47062afcb80576ebdf30100010040ac270000000000000000000000000000000000000000000000000000000000085a580ec9b95ef21e0000
```

```json
{
  "hash": "9b50bf1ea887a54c17b72d010185714a226631d551f901dba8587b004b0f62ba",
  "type": 16,
  "timestamp": 1615869450,
  "remark": null,
  "txDataHex": "01000155b03381577d329bb52dd47062afcb80576ebdf3010002a443687a6883154a1a94e8b66419c1490716d0b20000000000000000000000000000000000000000000000000000000000000000a0860100000000001900000000000000087472616e73666572000201254e554c53643648675a7068736e3151734d6d414b3573613247724a3279704d4c3278524163010731303030303030",
  "from": [
    {
      "address": "NULSd6HgZ8xEbCKo9J5MwgJYVy9F3Cpzvh2GY",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "2600000",
      "nonce": "5a580ec9b95ef21e",
      "locked": 0
    }
  ],
  "to": [],
  "txData": {
    "sender": "NULSd6HgZ8xEbCKo9J5MwgJYVy9F3Cpzvh2GY",
    "contractAddress": "NULSd6HguiDcyABtp3RGKXk2sEVXK2y8At4hq",
    "value": "0",
    "gasLimit": 100000,
    "price": 25,
    "methodName": "transfer",
    "methodDesc": "",
    "args": [
      [
        "NULSd6HgZphsn1QsMmAK5sa2GrJ2ypML2xRAc"
      ],
      [
        "1000000"
      ]
    ]
  }
}
```

#### Transaction Demonstration (2)

Common calling contract

```json
testing feedback message: e006000096058000002c8000003c80000000000000000000000010007179fe63006e01000159cee52b78b1083cc4491a7b1bfb088489532cf901000299e3598aa45be4c399b12ca3913bf880d4055ec800902f50090000000000000000000000000000000000000000000000000000000d8501000000000019000000000000000d6465706f736974466f724f776e00008c011701000159cee52b78b1083cc4491a7b1bfb088489532cf901000100e51457500900000000000000000000000000000000000000000000000000000008fe6f80bacf5c363500011701000299e3598aa45be4c399b12ca3913bf880d4055ec80100010000902f50090000000000000000000000000000000000000000000000000000000000000000000000
```

```json
{
  "hash": "bc6fbdf8c5af3d15c93ae908d64dc57abb9798bdd68d8b9ffda7ab9c175a710d",
  "type": 16,
  "timestamp": 1677621617,
  "remark": null,
  "transactionSignature": "210229a3123fc78e12fb1f68424ee361c6e2fe0a38586ada3ae466ff03eeada7f12e473045022100d0a1f903359dd25bec3a58286dc511a90da82ec13b6c36df4cbdce82c20b7d3a02206590acbc200178e9f89190fb875eef9b053221e8f5635a556343d7bd742cd913",
  "txDataHex": "01000159cee52b78b1083cc4491a7b1bfb088489532cf901000299e3598aa45be4c399b12ca3913bf880d4055ec800902f50090000000000000000000000000000000000000000000000000000000d8501000000000019000000000000000d6465706f736974466f724f776e0000",
  "from": [
    {
      "address": "NULSd6HgZPeU4ciTWF3XnQgxgtK7ch8unRhfY",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "40002589925",
      "nonce": "fe6f80bacf5c3635",
      "locked": 0
    }
  ],
  "to": [
    {
      "address": "NULSd6Hgu5DRgAg11zwyq1C1NGpKBGYUm3Uhu",
      "assetsChainId": 1,
      "assetsId": 1,
      "amount": "40000000000",
      "lockTime": 0
    }
  ],
  "txData": {
    "sender": "NULSd6HgZPeU4ciTWF3XnQgxgtK7ch8unRhfY",
    "contractAddress": "NULSd6Hgu5DRgAg11zwyq1C1NGpKBGYUm3Uhu",
    "value": "40000000000",
    "gasLimit": 99597,
    "price": 25,
    "methodName": "depositForOwn",
    "methodDesc": "",
    "args": []
  }
}
```

### Other Transactions

#### Display Items

|Title|Value|Description|
|:----|:----|:----|
|Type|Blind sign|blind signature type|
|Amount|NULS 8.88 / 9-55 898989|amount（refer to the "Note" below）|
|Address|NULSd.... / NERVEe...|to address|
|Fee|NULS 0.088|transaction fee|

## Method to Calculate Transaction Fee

Parse the CoinData to obtain the collections of “CoinFrom” and “CoinTo”；

Sum up the amount of all assets with asset ID 1-1 in “CoinFrom” and assign it to variable A；

Sum up all amounts with AssetID 1-1 in the “CoinTo” collection, and let it be denoted as B；

 `Fee = A - B` 

Note：Proceed a null check when transactions without CoinTo collection and CoinData occur.
## Internal Assets Info

When parsing a transaction, if an internal asset is matched, the amount should be displayed with precision modification.

### NULS Assets

|**AssetId**|**Symbol**|**Decimals**|
|:----|:----|:----|
|1-1|NULS|8|

### Nerve Crosschain Assets

|**AssetId**|**Symbol**|**Decimals**|
|:----|:----|:----|
|9-1|NVT|8|
|9-220|USDTN|18|
|9-388|USDCN|18|

### NRC20 Assets

|**Contract Address**|**Symbol**|**Decimals**|
|:----|:----|:----|
|NULSd6Hgv3Y49CHapt5qL4pCiEX8x3ZHrh6ie|NABOX|18|
|NULSd6HgrzcXdTuGvRF9DmkxoXM8XNmXZs95d|NSWAP|8|
|NULSd6Hgr7kEY9dEJVEpjsWAgmGRqBiMe9vL7|WTF|8|
|NULSd6HgvhaiirRcbg3QY7GWSU6RSFGaVovme|BULS|8|
|NULSd6Hh1AW8WxfB64RW1EYxP4zXaMxkQi5xd|NFTC|2|



