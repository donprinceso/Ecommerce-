<?php

namespace application\security;


use Exception;
use stdClass;

class APIs
{

    function current_bitcoin_price():stdClass{
        error_reporting(0);
        $result = new stdClass();
        try {
            $data = json_decode(file_get_contents("https://widgets.coinmarketcap.com/v2/ticker/1/?ref=widget&convert=USD"));
            $price = $data->data;
            $a = $price->quotes->USD;
            $b = $price->quotes->BTC;
            $usdprice = $a->price;
            $btcprice = $b->price;
        }catch (Exception $ex){
            $result->error = $ex;
            $usdprice = "0.00";
            $btcprice = "0.00";
        }
        $result->usd = $usdprice;
        $result->btc = $btcprice;
        return $result;
    }

}