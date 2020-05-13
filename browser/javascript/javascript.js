'use strict';

/**This is the Ajax handler**/
$(function () {

    let form = function ($this,options) {

        let defaults = {
            keyword:'tele_tech',
            type:'POST',
            addformData:[],
            url:'server',
            dataType:null,
            setup:function () {},
            before:function () {},
            success:function(a,b){},
            fail:function(a,b){},
            always:function(){}
        };


        options = $.extend({},defaults,options);

        function submit() {
            $this.on('submit',function (e) {
                e.preventDefault();

                options.setup.call();

                let form = new FormData(this);
                for(let i=0; i<options.addformData.length; i++){
                    form.append(
                        options.addformData[i][0],
                        options.addformData[i][1]
                    );
                }
                form.append(options.keyword,Date.now());

                options.before.call();

                $.ajax({
                    type: options.type,
                    processData: false,
                    contentType: false,
                    url: options.url,
                    cache: false,
                    data: form,
                    dataType: options.dataType,
                    encode: true
                }).done(function(a,b){options.success(a,b)})
                    .fail(function(a,b){options.fail(a,b)})
                    .always(function(){options.always()});
            });
        }

        return submit.call(this);

    };

    $.fn.xsubmit = function (opt) {
        return this.each(function () {
            new form($(this),opt);
        });
    }

});

$(document).ready(function () {

    function PNotifyPop(title, message,type) {
        new PNotify({
            title: title,
            text: message,
            cornerclass: 'ui-pnotify-sharp',
            type: type,
            buttons: {
                closer: false,
                sticker: false
            }
        });
    }

    function SweetAlertPop(ttitle, mmessage,ttype,onclose) {
        Swal.fire({
            title: ttitle,
            html: mmessage,
            type: ttype,
            onClose: onclose
        });
    }

    function SweetAlertQuestion(title,text,type,result){
        swalWithBootstrapButtons.fire({
            title,
            text,
            type,
            showCancelButton: true,
            confirmButtonText: 'Yes',
            cancelButtonText: 'No',
            reverseButtons: true
        }).then(result);
    }

    function consume_alert() {
        let _alert = window.alert;
        window.alert = function(message) {
            new PNotify({
                title: 'Alert',
                text: message
            });
        };
    }

    //change the alert function to use PNotify Plugin
    consume_alert();

    let body = $("body");
    let page = body.attr("app-page");

    let login = ()=>{

        let from_login = body.find("[customer_login]");

        let login_submit = from_login.find("[type='submit']");
        let login_email = from_login.find("[name='email']");
        let login_password = from_login.find("[name='password']");

        from_login.xsubmit({
            dataType:"json",
            addformData:[
                ['customer-login','']
            ],
            success: function (a) {
                if (a.success) {
                    preventDefault();
                    PNotifyPop("Login Successfully", a.info, 'success');
                    setTimeout(()=>{
                        location.reload();
                    },1000);
                } else {
                    PNotifyPop("login failed ", a.info, 'error');
                }
            }, fail: function (a, b) {
                PNotifyPop("Login fail", "An Error Occurred", 'error');
            },
        });
    };


    let register = ()=>{
        let body = $("body");
        let from_register = body.find("from[customer_register]");

        let register_submit = from_register.find("[type='subit");
        let sign_up_firstname = from_register.find("name='firstname'");
        let sign_up_lastname = from_register.find("name='lastname'");
        let sign_up_email = from_register.find("name='email'");
        let sign_up_password = from_register.find("name='password'");

        from_register.xsubmit({
           dataType:"json",
           addfromData:[['customer_register','']],
           success:function(a){
               if(a.success){
                   preventDefault();
                   PNotifyPop("Registration was successfully!!!",a.info,'success');
                   setTimeout(()=>{
                       location.reload();
                   },1000);
               }else{
                   PNotifyPop("registration fail",a.info,'error');
               }
           } ,fail:function (a,b)  {
                PNotifyPop("registration fail",a.info,'error');
            }
        });
    };

    let settings = ()=>{
        switch(page){
            case "signin":
                login();
                break;
            case "signup":
                register();
                break;
        }
    };


    settings();

});
