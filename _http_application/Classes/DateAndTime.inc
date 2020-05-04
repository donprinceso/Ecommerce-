<?php

namespace application\init;

class DateAndTime{

    public function CurrentTimestamp(){
        return time();
    }

    public function DateToStamp(string $date):int{
        $date = str_replace(['/',' ','\\',','],'-',$date);
        return strtotime($date);
    }

}