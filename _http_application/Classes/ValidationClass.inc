<?php
namespace application\security;



class ValidationClass{

    use TValidation;

    function __construct(int...$_){

    }

    function __invoke($input,int $f=ValidationModes::strUCLC_num, int...$_):bool{
        return $this->_check($input,$f,$_);
    }

    public function CheckAnd($input,int $f=ValidationModes::strUCLC_num, int...$_):bool{
        return $this->_check($input,$f,$_);
    }

    public function CheckOr($input,int $f=ValidationModes::strUCLC_num, int...$_):bool{
        return $this->_check($input,$f,$_);
    }

    public function CheckCustom($input,$f,...$_){
        $cvalues = array_merge([$f],$_);
        $d = implode($cvalues);
        return $this->_checkCustom($input,$d);
    }

    public function Matches():array {
        return $this->matches;
    }



    private function _check($input,int $f=ValidationModes::strUCLC_num,$_){
        $result = true;
        $checker = $this->checker(array_merge([$f],$_));
        foreach ($checker as $value){
            $result = ($this->validating($input,$value));
            if($result==false)break;
        }
        return $result;
    }

    private function _checkCustom($input,$checker,$min=0,$max=null){
        $max = (is_null($max))?"":$max;
        $reg = "/^[$checker]{".$min.",".$max."}+$/";
        return !!preg_match($reg,$input,$this->matches);
    }

    private function _checkPattern($input,$checker){
        $reg = "/^[$checker]{1,}+$/";
        return !!preg_match($reg,$input,$this->matches);
    }

    private function validating($val, string $reg):bool {
        return !!preg_match($reg,$val,$this->matches);
    }

    private function checker(array $_=[]):array{
        $result = [];
        $data = $this->knowledge_base();
        foreach ($_ as $value){
            if(array_key_exists($value,$data)){
                $result[] = $data[$value];
            }
        }
        return $result;
    }

}
