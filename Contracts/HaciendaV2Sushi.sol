pragma solidity >= 0.6.6;
pragma experimental ABIEncoderV2;

import '@sushiswap/core/contracts/uniswapv2/libraries/UniswapV2Library.sol';
import '@sushiswap/core/contracts/uniswapv2/interfaces/IUniswapV2Pair.sol';
import '@sushiswap/core/contracts/uniswapv2/interfaces/IUniswapV2Factory.sol';


contract HaciendaV2Sushi {


    uint private MaxPairs = 35000;

    address private SushiFactory = 0xc35DADB65012eC5796536bD9864eD8773aBc74C4;

    //address[] private pairs;

    // Temporary data, structs cannot be returned.
    struct HaciendaResponse {
        address pairAddr;
        address token0;
        address token1;

        uint reserve0;
        uint reserve1;

        uint decimals;

    }

    function haciendaInfo(address factory, address tokenA, address tokenB) external view returns (address pairAddr, address token0, address token1,
        uint reserve0, uint reserve1, uint decimals) {
        HaciendaResponse memory temp;


        temp.pairAddr = UniswapV2Library.pairFor(factory, tokenA, tokenB);
        IUniswapV2Pair pairObj = IUniswapV2Pair(temp.pairAddr);

        //Get reserves
        uint blockNotNeeded;
        (temp.reserve0, temp.reserve1, blockNotNeeded) = pairObj.getReserves();

        //Decimals
        temp.decimals = pairObj.decimals();

        //token0 and token1 in order
        temp.token0 = pairObj.token0();
        temp.token1 = pairObj.token1();

        // asign to return
        pairAddr = temp.pairAddr;
        token0 = temp.token0;
        token1 = temp.token1;

        reserve0 = temp.reserve0;
        reserve1 = temp.reserve1;
        decimals = temp.decimals;
    }

    function getAllPairs(uint fromIndex, uint toPosition) external view returns (string[] memory) {

        // do the magic (de 10 en 10)

        string[] memory pairs = new string[](10);

        uint count = 0;
        IUniswapV2Factory factory = IUniswapV2Factory(SushiFactory);

        for (uint i = fromIndex; i < toPosition; i++) {

            try factory.allPairs(i) returns (address pair)
            {
                IUniswapV2Pair pairObj = IUniswapV2Pair(pair);

                string memory pairString = strConcat("0x", toAsciiString(pair));
                string memory token0 = strConcat("0x", toAsciiString(pairObj.token0()));
                string memory token1 = strConcat("0x", toAsciiString(pairObj.token1()));

                pairs[count] = strConcat(pairString, "/", token0, token1);

            }

            catch (bytes memory reason)
            {
                break;
            }

            count = count + 1;
        }

        return pairs;
    }


    function pairReserves(address pairAddr) external view returns (uint reserve0, uint reserve1) {

        IUniswapV2Pair pairObj = IUniswapV2Pair(pairAddr); // Make this fast

        //Get reserves and return them
        uint blockNotNeeded;
        (reserve0, reserve1, blockNotNeeded) = pairObj.getReserves();
    }

    function toAsciiString(address x) internal pure returns (string memory) {
        bytes memory s = new bytes(40);
        for (uint i = 0; i < 20; i++) {
            bytes1 b = bytes1(uint8(uint(uint160(x)) / (2**(8*(19 - i)))));
            bytes1 hi = bytes1(uint8(b) / 16);
            bytes1 lo = bytes1(uint8(b) - 16 * uint8(hi));
            s[2*i] = char(hi);
            s[2*i+1] = char(lo);
        }
        return string(s);
    }

    function char(bytes1 b) internal pure returns (bytes1 c) {
        if (uint8(b) < 10) return bytes1(uint8(b) + 0x30);
        else return bytes1(uint8(b) + 0x57);
    }

    function uint2str(uint _i) internal pure returns (string memory _uintAsString) {
        if (_i == 0) {
            return "0";
        }
        uint j = _i;
        uint len;
        while (j != 0) {
            len++;
            j /= 10;
        }
        bytes memory bstr = new bytes(len);
        uint k = len - 1;
        while (_i != 0) {
            bstr[k--] = byte(uint8(48 + _i % 10));
            _i /= 10;
        }
        return string(bstr);
    }

    function strConcat(string memory _a, string memory _b, string memory _c, string memory _d, string memory _e) internal view returns (string memory){
        bytes memory _ba = bytes(_a);
        bytes memory _bb = bytes(_b);
        bytes memory _bc = bytes(_c);
        bytes memory _bd = bytes(_d);
        bytes memory _be = bytes(_e);
        string memory abcde = new string(_ba.length + _bb.length + _bc.length + _bd.length + _be.length);
        bytes memory babcde = bytes(abcde);
        uint k = 0;
        for (uint i = 0; i < _ba.length; i++) babcde[k++] = _ba[i];
        for (uint i = 0; i < _bb.length; i++) babcde[k++] = _bb[i];
        for (uint i = 0; i < _bc.length; i++) babcde[k++] = _bc[i];
        for (uint i = 0; i < _bd.length; i++) babcde[k++] = _bd[i];
        for (uint i = 0; i < _be.length; i++) babcde[k++] = _be[i];
        return string(babcde);
    }

    function strConcat(string memory _a, string memory _b, string memory _c, string memory _d) internal view returns (string memory) {
        return strConcat(_a, _b, _c, _d, "");
    }

    function strConcat(string memory _a, string memory _b, string memory _c) internal view returns (string memory) {
        return strConcat(_a, _b, _c, "", "");
    }

    function strConcat(string memory _a, string memory _b) internal view returns (string memory) {
        return strConcat(_a, _b, "", "", "");
    }

}