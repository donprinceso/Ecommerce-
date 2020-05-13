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

    function PNotifyPop(title, message, type) {
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

    function consume_alert() {
        let _alert = window.alert;
        window.alert = function (message) {
            new PNotify({
                title: 'Alert',
                text: message
            });
        };
    }

    //change the alert function to use PNotify Plugin
    consume_alert();


    let adminLogin = ()=>{
        let body = $('body');
        let from_login = body.find("[admin-login]");

        let login_submit = from_login.find("[type='submit']");
        let login_email = from_login.find("[name='email']");
        let login_password = from_login.find("[name='password']");

        from_login.xsubmit({
            dataType:"json",
            addformData:[
                ['admin---login','']
            ],
            before: function(){
                login_submit.append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
            },
            success: function (a) {
                if (a.success) {
                    PNotifyPop("Login Successfully!!!",a.info,'success');
                    setTimeout(()=>{
                        location.reload('');
                    },1000);
                } else {
                    login_email.parent().addClass('has-error');
                    login_password.parent().addClass('has-error');
                    PNotifyPop("login failed ", a.info, 'error');
                }
            }, fail: function (a) {
                PNotifyPop("Login fail", "An Error Occurred", 'error');
            },always:()=>{
                setTimeout(()=>{
                    login_submit.find(".b-loader").remove();
                },1000);
            }
        });
    };


    adminLogin();



});