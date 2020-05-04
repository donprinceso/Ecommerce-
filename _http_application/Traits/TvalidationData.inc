<?php

namespace application\security;

abstract class ValidationModes{
    public const strUCLC_num = 1;
    public const strUC_num = 2;
    public const strLC_num = 3;
    public const strUCLC = 4;
    public const strUC = 5;
    public const strLC = 6;
    public const num = 7;
    public const fname_lname = 8;
    public const all_utf8 = 9;
}

trait TValidation{
    private $matches = [];
    private $strUCLC_num_val = "/^[A-Za-z0-9]{1,}+$/";
    private $strUC_num_val = "/^[A-Z0-9]{1,}+$/";
    private $strLC_num_val = "/^[a-z0-9]{1,}+$/";
    private $strUCLC_val = "/^[A-Za-z]{1,}+$/";
    private $strUC_val = "/^[A-Z]{1,}+$/";
    private $strLC_val = "/^[a-z]{1,}+$/";
    private $num_val = "/^[0-9]{1,}+$/";
    private $fname_lname_val = "/^[A-Za-z]{3,100} [A-Za-z]{3,100}+$/";
    private $all_utf8_val = "#^[ -~£±§]*$#";

    private function knowledge_base():array {
        return [
            ValidationModes::strUCLC_num => $this->strUCLC_num_val,
            ValidationModes::strUC_num => $this->strUC_num_val,
            ValidationModes::strLC_num => $this->strLC_num_val,
            ValidationModes::strUCLC => $this->strUCLC_val,
            ValidationModes::strUC => $this->strUC_val,
            ValidationModes::strLC => $this->strLC_val,
            ValidationModes::fname_lname => $this->fname_lname_val,
            ValidationModes::all_utf8 => $this->all_utf8_val,
        ];
    }

}