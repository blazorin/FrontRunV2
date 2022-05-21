pragma solidity >= 0.6.6;

import '@uniswap/v2-periphery/contracts/libraries/UniswapV2Library.sol';
import '@uniswap/v2-core/contracts/interfaces/IUniswapV2Pair.sol';
import '@uniswap/v2-periphery/contracts/interfaces/IUniswapV2Router02.sol';
import '@uniswap/v2-core/contracts/interfaces/IUniswapV2ERC20.sol';


contract HaciendaUtility {


    address private QuickswapRouterAddr = 0xa5E0829CaCEd8fFDD4De3c43696c57F7D7A678ff;
    address private BaseToken = 0x0d500B1d8E8eF31E21C99d1Db9A6444d3ADf1270; //wmatic
    //address private owner = 0x072B0922A3AbC86D56f3fE853B3A458f1814F6D4;
    uint private BaseTokenAmount = 1; //wmatic (honeypot)
    uint private StableCoinAmount = 1; //stablecoin (honeypot)

    // Teporary data, structs cannot be returned.
    struct HaciendaResponse {
        address pairAddr;
        address token0;
        address token1;

        uint reserve0;
        uint reserve1;
        uint reserve0_temp;

        uint decimals;

        uint blockNotNeeded;

        address[] route;
        address[] route2;

    }

    function calculateMaxSlipagge_SwapTokensForExactTokens(uint maxInput, uint Out, uint price) internal pure returns (uint maxSlippage) {

        uint input_without_slippage = Out * price;

        maxSlippage = ((maxInput - input_without_slippage) / input_without_slippage);
    }


    function calculateMaxSlipagge(uint input, uint minOut, uint price) internal pure returns (uint maxSlippage) {

        uint ammount_without_slippage = input / price;

        maxSlippage = ((ammount_without_slippage - minOut)/ammount_without_slippage);
    }


    function calculateMaxEntry(uint maxSlippage, uint pairReserve0) internal pure returns (uint maxEntry) {

        maxSlippage = maxSlippage * 1;
        pairReserve0 = 1;

        return 1;
    }

    function haciendaInfo(address factory, address tokenA, address tokenB) external returns (address pairAddr, address token0, address token1,
        uint reserve0, uint reserve1, uint decimals, bool honeyPot) {
        HaciendaResponse memory temp;


        temp.pairAddr = UniswapV2Library.pairFor(factory, tokenA, tokenB);
        IUniswapV2Pair pairObj = IUniswapV2Pair(temp.pairAddr);

        //Get reserves
        (temp.reserve0, temp.reserve1, temp.blockNotNeeded) = pairObj.getReserves();

        //Decimals
        temp.decimals = pairObj.decimals();

        //token0 and token1 in order
        temp.token0 = pairObj.token0();
        temp.token1 = pairObj.token1();

        if(tokenA != temp.token0 && tokenA == temp.token1){
            temp.reserve0_temp = temp.reserve0;
            temp.reserve0 = temp.reserve1;
            temp.reserve1 = temp.reserve0_temp;
        }


        // HoneyPot check
        IUniswapV2Router02 router = IUniswapV2Router02(QuickswapRouterAddr);


        /*
        uint maxSlippage = 0;
        if (!swapTokensStandard) {
            maxSlippage = calculateMaxSlipagge_SwapTokensForExactTokens(amountToken0, amountToken1, price);
        } else {
           maxSlippage = calculateMaxSlipagge(amountToken0, amountToken1, price);
        }
        */


        uint attackAmmount = IUniswapV2ERC20(tokenA).balanceOf(msg.sender);
        //Just to avoid errors
        if(attackAmmount <= 0){
            attackAmmount = 1;
        }


        uint attack_token_1 = attackAmmount / ( (temp.reserve0 * (10**temp.decimals)) / temp.reserve1);
        //attack_token_1 = temp.reserve0;
        attack_token_1 = attack_token_1 * (10**temp.decimals);


        //uint amountToken1 =
        //Transferimos el token de la wallet a el contrato
        IUniswapV2ERC20(tokenA).transferFrom(msg.sender, address(this), attackAmmount);
        //Aprovamos los tokens para poder hacer el swap.
        IUniswapV2ERC20(tokenA).approve(address(QuickswapRouterAddr), 29642774844752946028434172162224104410437116074403984394101141506025761187823);
        IUniswapV2ERC20(tokenB).approve(address(QuickswapRouterAddr), 29642774844752946028434172162224104410437116074403984394101141506025761187823);

        temp.route = new address[](2);
        temp.route[0] = tokenB;
        temp.route[1] = tokenA;

        temp.route2 = new address[](2);
        temp.route2[0] = tokenA;
        temp.route2[1] = tokenB;

        //IERC20(tokenA).approve(address(QuickswapRouterAddr), 1 * (10**23));
        //IERC20(tokenB).approve(address(QuickswapRouterAddr), 1 * (10**23));

        ///uint256[] memory result;

        uint256[] memory data2;
        data2 = router.swapExactTokensForTokens(attackAmmount, attack_token_1/2, temp.route2, address(this), block.timestamp);
        attack_token_1 = IUniswapV2ERC20(tokenB).balanceOf(address(this));
        data2 = router.swapExactTokensForTokens(attack_token_1, 0, temp.route, address(this), block.timestamp);


        //Return the balance of tokeno to the wallet.
        uint resultAmmount = IUniswapV2ERC20(tokenA).balanceOf(address(this));

        IUniswapV2ERC20(tokenA).transfer(msg.sender, resultAmmount);

        if( !(resultAmmount >= 0)){
            honeyPot = true;
        }


        // asign to return
        pairAddr = temp.pairAddr;
        token0 = temp.token0;
        token1 = temp.token1;

        reserve0 = temp.reserve0;
        reserve1 = temp.reserve1;
        decimals = temp.decimals;
    }

}