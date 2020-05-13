'use strict';

let Settings = {
    appName: "Tele Tech Ltd",
    AcceptedImages: {
        image: "image/jpg,image/jpeg,image/png",
        imageArray: ()=>{
            return Settings.AcceptedImages.image.split(",");
        },
        maxsize: 1024*10,
        maxwidth: 500,
        maxheight: 500
    },
    FileLinks:{
        brand: "browser/dash/images/brand-icon.png",
    }
};

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

    function SweetAlertPop(ttitle, mmessage, ttype, onclose) {
        Swal.fire({
            title: ttitle,
            html: mmessage,
            type: ttype,
            onClose: onclose
        });
    }
    const swalWithBootstrapButtons = Swal.mixin({
        customClass: {
            confirmButton: 'btn btn-success',
            cancelButton: 'btn btn-danger'
        },
        buttonsStyling: false,
    });

    function SweetAlertQuestion(title, text, type, result) {
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
        window.alert = function (message) {
            new PNotify({
                title: 'Alert',
                text: message
            });
        };
    }

    //change the alert function to use PNotify Plugin
    consume_alert();


    /***THE MAIN ==================================***/
    let body = $("body");
    let navigation = body.find("[app-navigation]");
    let pageContent = body.find("[app-page-content]");


    let FileLinks = Settings.FileLinks;

    let General = ()=>{

        let Navigation = ()=>{
            let items = navigation.find("[app-link]");

            let pageLoader = ()=>{
                return `<div class="page-loader">
                            <i class="loader-icon fa fa-spin fa-spinner"></i>
                        </div>`;
            };
            let pageExist = ()=>{
                return `<div>
                            Page does not exist
                        </div>`;
            };
            let pageNetwork = ()=>{
                return `<div>
                            no network found
                        </div>`;
            };

            items.on("click", function (e) {
                e.preventDefault();
                e.stopPropagation();

                let $this = $(this);
                let name = $this.attr("app-link");
                /***dom handler===================**/
                items.parent("li").removeClass("active");
                $this.parent("li").addClass("active");

                /***server handler===================**/
                let form = $("<form/>");
                form.xsubmit({
                    addformData:[
                        ['admin-pages',''],
                        ['page',name]
                    ],
                    before: function(){
                        pageContent.html(pageLoader());
                    },
                    success: function (a) {
                        setTimeout(()=>{
                            if(a.trim()==="page-not-exist-##"){
                                pageContent.html(pageExist());
                            }else{
                                pageContent.html(a);
                                PageAction(name);
                            }
                        },1000);
                    }, fail: function (a) {
                        pageContent.html(pageNetwork());
                    }
                });
                form.trigger("submit");
            });

            items.first().trigger("click");
        };

        let PageAction = (page)=>{
            switch (page) {
                case "admin_dashboard":
                    pageDashboard();
                    break;
                case "admin_order":
                    pageOrder();
                    break;
                case "admin_brand_add":
                    pageBrandAdd();
                    break;
                case "admin_brand_list":
                    pageBrandList();
                    break;
                case "admin_model_add":
                    pageModelAdd();
                    break;
                case "admin_model_list":
                    pageModelList();
                    break;
                case "admin_customer_list":
                    pageCustomer();
                    break;
                case "admin_password":
                    pagePassword();
                    break;
            }
        };

        Navigation();
    };


    /***PAGES ACTION BEGIN ============================***/
    let pageDashboard = ()=>{

    };

    let pageOrder = ()=>{

    };

    /***BRAND PAGE ================***/
    let pageBrandAdd = ()=>{
        let Brand = pageContent.find("[app-brand]");

        let brandPreview = Brand.find("[app-brand-preview]");
        let brandFile = Brand.find("[app-brand-file]");
        let brandLink = Brand.find("[app-brand-link]");
        let brandLoader = Brand.find("[app-brand-loader]");

        brandPreview.on('click',function () {
            brandFile.trigger("click");
        });

        brandFile.on("change",function(){
            let file = this.files[0];
            if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                $("<form></form>").xsubmit({
                    dataType: 'json',
                    addformData: [
                        ['brand-image', ''],
                        ['file', file]
                    ],
                    before: function () {
                        brandLoader.addClass("show");
                    },
                    success: function (a) {
                        if(a.success){
                            let url = a.url;
                            let uploadurl = a.uploadURL;
                            brandPreview.attr("src",url);
                            brandLink.val(uploadurl);
                        }else{
                            PNotifyPop(Object.title,a.info,'error');
                        }
                    },
                    fail: function (a) {
                        PNotifyPop(Object.title,"An Error Occurred",'error');
                    },
                    always: function () {
                        setTimeout(function () {
                            brandLoader.removeClass("show");
                        }, 1000);
                    }
                }).trigger('submit');
            }else{
                PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
            }
        });

        let addBrand = ()=>{
            let form_add_brand = Brand.find("[app-brand-add]");

            let add_submit = form_add_brand.find("[type='submit']");
            let add_name = form_add_brand.find("[name='name']");
            let add_image = form_add_brand.find("[app-brand-file]");

            form_add_brand.xsubmit({
                dataType:"json",
                addformData:[
                    ['brand_add','']
                ],
                before: function(){
                    add_submit.append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
                },
                success: function (a) {
                    if (a.validated) {
                        if(a.success){
                            PNotifyPop(Settings.appName,a.info,'success');
                            brandPreview.attr("src",FileLinks.brand);
                            form_add_brand.trigger("reset");
                        }else{
                            PNotifyPop(Settings.appName,a.info,'error');
                        }
                    } else {
                        if(a.name===false){
                            add_name.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.name_t,'error');
                        }
                        if(a.filename===false){
                            PNotifyPop(Settings.appName, a.filename_t,'error');
                            brandFile.trigger("click");
                        }
                    }
                }, fail: function (a) {
                    PNotifyPop(Settings.appName, "An Error Occurred", 'error');
                },always:()=>{
                    setTimeout(()=>{
                        add_submit.find(".b-loader").remove();
                    },1000);
                }
            });
        };

        addBrand();

    };

    let pageBrandList = ()=>{
        let Brand = pageContent.find("[app-brand]");
        let BrandEditBox = Brand.find("[app-edit-box]");

        let BrandEdit = ()=>{
            let brandeditBtn = Brand.find("[app-edit]");
            let updateForm = BrandEditBox.find("[app-brand-update]");
            let brandImage = updateForm.find("[app-brand-preview]");
            let brandName = updateForm.find("[name='name']");
            let brandId = updateForm.find("[app-brand-id]");
            let brandLink = updateForm.find("[app-brand-link]");

            let brandLoader = updateForm.find("[app-brand-loader]");


            let brandFile = updateForm.find("[app-brand-file]");
                // for image getting
            brandImage.on("click",()=>brandFile.trigger("click"));

            // for image uploading
            brandFile.on("change",function () {
                let file = this.files[0];
                if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                    $("<form></form>").xsubmit({
                        dataType: 'json',
                        addformData: [
                            ['brand-image', ''],
                            ['file', file]
                        ],
                        before: function () {
                            brandLoader.addClass("show");
                        },
                        success: function (a) {
                            if(a.success){
                                let url = a.url;
                                let uploadurl = a.uploadURL;
                                brandImage.attr("src",url);
                                brandLink.val(uploadurl);
                            }else{
                                PNotifyPop(Object.title,a.info,'error');
                            }
                        },
                        fail: function (a) {
                            PNotifyPop(Object.title,"An Error Occurred",'error');
                        },
                        always: function () {
                            setTimeout(function () {
                                brandLoader.removeClass("show");
                            }, 1000);
                        }
                    }).trigger('submit');
                }else{
                    PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                }
            });


            brandeditBtn.on("click",function(){
                let $this = $(this);
                let row = $this.closest("[app-brand-item]");
                let id = row.attr("app-brand-item");
                $("<form/>").xsubmit({
                    dataType: 'json',
                    addformData: [
                        ['view-brand', ''],
                        ['id', id]
                    ],
                    before: function () {
                        brandName.val("");
                        brandImage.attr("src",FileLinks.brand);
                        brandLink.val("");
                    },
                    success: function(a){
                        setTimeout(()=>{
                            if(a.found) {
                                let data = a.data;
                                brandImage.attr("src",data.image);
                                brandId.val(data.id);
                                brandName.val(data.name);
                                brandLink.val(data.url);
                                BrandEditBox.modal('show');
                            }else{
                                PNotifyPop(Settings.appName, a.info,'error');
                            }
                        },1000);
                    },fail:function () {
                        PNotifyPop(Settings.appName, "An error occurred",'error');
                    }
                }).trigger("submit");
            });

            let UpdateBrand = ()=>{
                let form_update_brand = Brand.find("[app-brand-update]");

                let update_submit = form_update_brand.find("[type='submit']");
                let update_name = form_update_brand.find("[name='name']");
                let update_image = form_update_brand.find("[app-brand-file]");


                form_update_brand.xsubmit({
                    dataType:"json",
                    addformData:[
                        ['Brand-update',''],
                    ],
                    before: function(){
                        update_submit.append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
                    },
                    success: function (a) {
                            if(a.success){
                                PNotifyPop(Settings.appName,a.info,'success');
                                brandImage.attr("src",FileLinks.brand);
                                form_update_brand.trigger("reset");

                                let row = Brand.find("[app-brand-item='"+a.dataID+"']");
                                row.find("[app-update-filename]").attr('src',a.dataImage);
                                row.find("[app-update-name]").text(a.dataName);
                                row.find("[app-update-update]").text(a.dataUpdate);

                                brandLoader.addClass("hide");
                                BrandEditBox.modal("hide");

                            }else{
                                PNotifyPop(Settings.appName,a.info,'error');
                            }
                    }, fail: function (a) {
                        PNotifyPop(Settings.appName, "An Error Occurred", 'error');
                    },always:()=>{
                        setTimeout(()=>{
                            update_submit.find(".b-loader").remove();
                        },1000);
                    }
                });
            };

            UpdateBrand();

        };

        let BrandDelete = ()=>{
            let brandeditBtn = Brand.find("[app-delete]");

            brandeditBtn.on("click",function () {
                let $this = $(this);
                let row = $this.closest("[app-brand-item]");
                let id = row.attr("app-brand-item");
                SweetAlertQuestion(Settings.appName,"Are you sure want to delete?","question",(res)=>{
                    if (res.value) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['delete-Brand', ''],
                                ['id', id]
                            ],
                            success: (a) => {
                                if (a.success) {
                                    swalWithBootstrapButtons.fire(
                                        'Deleted!',
                                        'Brand deleted.',
                                        'success'
                                    );
                                    Brand.find("[app-brand-item='"+id+"']").remove();

                                } else {
                                    swalWithBootstrapButtons.fire(
                                        'Cancelled',
                                        a.info,
                                        'error'
                                    );
                                }
                            },
                            fail: () => {
                                swalWithBootstrapButtons.fire(
                                    'Cancelled',
                                    'An error Occurred',
                                    'error'
                                )
                            }
                        }).trigger('submit');
                    }
                });
            });
        };



        BrandEdit();
        BrandDelete();

    };

    /***MODEL PAGE =================***/
    let pageModelAdd = ()=>{
        let Model = pageContent.find("[app-model]");

        let addModel = ()=>{
            let formAdd = Model.find("[app-model-add]");

            let add_submit = formAdd.find("[type='submit']");
            let add_brand = formAdd.find("[name='brand']");
            let add_name = formAdd.find("[name='name']");

            formAdd.xsubmit({
                dataType:"json",
                addformData:[
                    ['model_add','']
                ],
                before: function(){
                    add_submit.append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
                },
                success: function (a) {
                    if (a.validated) {
                        if(a.success){
                            PNotifyPop(Settings.appName,a.info,'success');
                            formAdd.trigger("reset");
                        }else{
                            PNotifyPop(Settings.appName,a.info,'error');
                        }
                    } else {
                        if(a.brand===false){
                            add_brand.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.brand_t, 'error');
                        }
                        if(a.name===false){
                            add_name.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.name_t, 'error');
                        }
                    }
                }, fail: function (a) {
                    PNotifyPop(Settings.appName, "An Error Occurred", 'error');
                },always:()=>{
                    setTimeout(()=>{
                        add_submit.find(".b-loader").remove();
                    },1000);
                }
            });
        };

        addModel();

    };

    let pageModelList = ()=>{
            let Model = pageContent.find("[app-model]");
            let ModelEditBox = Model.find("[app-edit-box]");

            let ModelEdit = ()=>{
                let modeleditBtn = Model.find("[app-edit]");
                let updateForm = ModelEditBox.find("[app-model-update]");
                let modelName = updateForm.find("[name='name']");
                let modelId = updateForm.find("[app-model-id]");
                let modelbrand = updateForm.find("[app-model-brand]");

                modeleditBtn.on("click", function () {
                let $this = $(this);
                let row = $this.closest("[app-model-id]");
                let id = row.attr("app-model-id");
                    $("<form/>").xsubmit({
                        dataType: 'json',
                        addformData: [
                            ['view-model', ''],
                            ['id', id]
                        ],
                        before: function () {
                        },
                        success: function(a){
                            setTimeout(()=>{
                                if(a.found) {
                                    let data = a.data;
                                    modelId.val(data.id);
                                    modelName.val(data.name);
                                    modelbrand.val(data.brand);
                                    ModelEditBox.modal('show');
                                }else{
                                    PNotifyPop(Settings.appName, a.info,'error');
                                }
                            },1000);
                        },fail:function () {
                            PNotifyPop(Settings.appName, "An error occurred",'error');
                        }
                    }).trigger("submit");

                });
            };

        ModelEdit();
    };
    /***PASSWORD PAGE ===============***/
    let pagePassword = ()=>{
        let Password = pageContent.find("[app-password]");
    };


    /***CUSTOMER PAGE ===============***/
    let pageCustomer = ()=>{

    };

    /***PAGES ACTION END ============================***/


    General();



});