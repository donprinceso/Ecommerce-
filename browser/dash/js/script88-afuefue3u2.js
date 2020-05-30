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
        brand: "browser/images/brands/brand.jpg",
        model: "browser/images/models/model.jpg",
        model_front: "browser/images/models/model-front.jpg",
        model_back: "browser/images/models/model-back.jpg",
        model_left: "browser/images/models/model-left.jpg",
        model_right: "browser/images/models/model-right.jpg",
        model_top: "browser/images/models/model-top.jpg",
        model_bottom: "browser/images/models/model-bottom.jpg",
        avatar: "browser/images/avatar.png",
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
                case "admin_staff_add":
                    pageStaffAdd();
                    break;
                case "admin_staff_list":
                    pageStaffList();
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
                case "admin_jobs_add":
                    pageJobsAdd();
                    break;
                case "admin_jobs_list":
                    pageJobsList();
                    break;
                case "admin_customer_list":
                    pageCustomer();
                    break;
                case "admin_password":
                    pagePassword();
                    break;
            }
        };

        let Logout = ()=>{
            let logout = body.find("[app-logout]");
            logout.on("click", function(e){
                e.preventDefault();
                SweetAlertQuestion(Settings.appName,"Are you sure you want to logout?","question",(result) => {
                    if (result.value) {
                        $("<form/>").xsubmit({
                            dataType:'json',
                            addformData: [
                                ['signout','']
                            ],
                            before:() => { },
                            success: function (a) {
                                location.reload();
                            },
                            fail: function (a) {
                                SweetAlertPop(Object.title,"An error occurred",'error');
                            }
                        }).trigger('submit');
                    }
                });
            });
        };

        Logout();
        Navigation();
    };


    /***PAGES ACTION BEGIN ============================***/
    let pageDashboard = ()=>{

    };

    let pageOrder = ()=>{

    };


    /***THE STAFF SECTION ==========================**/
    let pageStaffAdd = ()=>{
        let Staff = pageContent.find("[app-staff]");

        let staffForm = Staff.find("[app-staff-add]");

        let avatarPreview = staffForm.find("[app-avatar-preview]");
        let avatarFile = staffForm.find("[app-avatar-file]");
        let avatarLoader = staffForm.find("[app-avatar-loader]");

        let avatarLink = staffForm.find("[name='avatar']");
        let name = staffForm.find("[name='name']");
        let gender = staffForm.find("[name='gender']");
        let maritalStatus = staffForm.find("[name='maritalStatus']");
        let email = staffForm.find("[name='email']");
        let phone = staffForm.find("[name='phone']");
        let ssn = staffForm.find("[name='ssn']");
        let country = staffForm.find("[name='country']");
        let city = staffForm.find("[name='city']");
        let residenceAddress = staffForm.find("[name='residenceAddress']");
        let submit = staffForm.find("[type='submit']");

        let UploadAvatar = ()=>{

            avatarPreview.on("click",()=>{
                avatarFile.trigger("click");
            });

            avatarFile.on("change",function(){
                let file = this.files[0];
                if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                    $("<form/>").xsubmit({
                        dataType: 'json',
                        addformData: [
                            ['staff-image', ''],
                            ['file', file]
                        ],
                        before: function () {
                            avatarLoader.addClass("show");
                        },
                        success: function (a) {
                            if(a.success){
                                let url = a.url;
                                let uploadurl = a.uploadURL;
                                avatarPreview.attr("src",url);
                                avatarLink.val(uploadurl);
                            }else{
                                PNotifyPop(Object.title,a.info,'error');
                            }
                        },
                        fail: function (a) {
                            PNotifyPop(Object.title,"An Error Occurred",'error');
                        },
                        always: function () {
                            setTimeout(function () {
                                avatarLoader.removeClass("show");
                            }, 1000);
                        }
                    }).trigger('submit');
                }else{
                    PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                }
            });

        };

        let SubmitForm = ()=>{
            staffForm.xsubmit({
                dataType: 'json',
                addformData: [
                    ['staff-add', '']
                ],
                before: function () {
                    submit.prop("disabled",true).append("<i class='b-loader fa fa-spin fa-spinner'></i>");
                    staffForm.find(".form-control").removeClass("form-control-danger");
                },
                success: function (a) {
                    if(a.validated){
                        if(a.success){
                            SweetAlertPop(Settings.title,a.info,'success');
                            staffForm.trigger("reset");
                            avatarPreview.attr("src",FileLinks.avatar);
                        }else{
                            SweetAlertPop(Settings.title,a.info,'error');
                        }
                    }else{
                        if(a.avatar===false){
                            PNotifyPop(Settings.appName,a.avatar_t,"error");
                        }
                        if(a.name===false){
                            name.addClass("form-control-danger");
                        }
                        if(a.gender===false){
                            gender.addClass("form-control-danger");
                        }
                        if(a.maritalStatus===false){
                            maritalStatus.addClass("form-control-danger");
                        }
                        if(a.email===false){
                            email.addClass("form-control-danger");
                            PNotifyPop(Settings.appName,a.email_t,"error");
                        }
                        if(a.phone===false){
                            phone.addClass("form-control-danger");
                            PNotifyPop(Settings.appName,a.phone_t,"error");
                        }
                        if(a.ssn===false){
                            ssn.addClass("form-control-danger");
                            PNotifyPop(Settings.appName,a.ssn_t,"error");
                        }
                        if(a.country===false){
                            country.addClass("form-control-danger");
                        }
                        if(a.city===false){
                            city.addClass("form-control-danger");
                        }
                        if(a.residenceAddress===false){
                            residenceAddress.addClass("form-control-danger");
                        }
                    }
                },
                fail: function (a) {
                    SweetAlertPop(Settings.title,"An error occurred",'error');
                },
                always: function () {
                    setTimeout(()=>{
                        submit.prop("disabled",false).find(".b-loader").remove();
                    },1000);
                }
            });
        };

        UploadAvatar();
        SubmitForm();
    };

    let pageStaffList = ()=>{
        let Staff = pageContent.find("[app-staff]");
        let Table = Staff.find("[app-table]");

        let View = ()=>{
            let ViewBtn = Staff.find("[app-view]");
            let ViewBox = Staff.find("[app-view-box]");

            let ViewTopImg = ViewBox.find("[app-top-view-avatar]");
            let ViewTopName = ViewBox.find("[app-top-view-name]");

            let ViewImage = ViewBox.find("[app-view-avatar]");
            let ViewName = ViewBox.find("[app-view-name]");
            let ViewGender = ViewBox.find("[app-view-gender]");
            let ViewMaritalStatus = ViewBox.find("[app-view-maritalstatus]");
            let ViewEmail = ViewBox.find("[app-view-email]");
            let ViewPhone = ViewBox.find("[app-view-phone]");
            let ViewSsn = ViewBox.find("[app-view-ssn]");
            let ViewCountry = ViewBox.find("[app-view-country]");
            let ViewCity = ViewBox.find("[app-view-city]");
            let ViewResidence = ViewBox.find("[app-view-residence]");
            let ViewUpdated = ViewBox.find("[app-view-updated]");
            let ViewCreated = ViewBox.find("[app-view-created]");

            let Clear = ()=>{
                ViewImage.attr("src",FileLinks.avatar);
                ViewTopImg.attr("src",FileLinks.avatar);
                ViewName.text("");
                ViewGender.text("");
                ViewMaritalStatus.text("");
                ViewEmail.text("");
                ViewPhone.text("");
                ViewSsn.text("");
                ViewCountry.text("");
                ViewCity.text("");
                ViewResidence.text("");
                ViewUpdated.text("N/A");
                ViewCreated.text("N/A");
            };
            let Load = (data)=>{
                let id = data.id;
                let avatar = data.avatar;
                let name = data.name;
                let gender = data.gender;
                let maritalStatus = data.maritalStatus;
                let email = data.email;
                let phone = data.phone;
                let ssn = data.ssn;
                let country = data.country;
                let city = data.city;
                let residence = data.residence;
                let updated = data.updated;
                let created = data.created;

                ViewTopName.text(name);
                ViewTopImg.attr("src",avatar);
                ViewImage.attr("src",avatar);
                ViewName.text(name);
                ViewGender.text(gender.toUpperCase());
                ViewMaritalStatus.text(maritalStatus.toUpperCase());
                ViewEmail.text(email);
                ViewPhone.text(phone);
                ViewSsn.text(ssn);
                ViewCountry.text(country);
                ViewCity.text(city);
                ViewResidence.text(residence);
                ViewUpdated.text(updated);
                ViewCreated.text(created);
            };

            ViewBtn.on("click",function(){
                let $this = $(this);
                let parent = $this.closest("[app-staff-id]");
                let id = parent.attr("app-staff-id");
                let form = $("<form/>");
                form.xsubmit({
                    dataType: 'json',
                    addformData: [
                        ['staff-view', ''],
                        ['id',id]
                    ],
                    before: function () {
                        Clear();
                        $this.prop("disabled",true);
                    },
                    success: function (a) {
                        if(a.found){
                            let data = a.data;
                            Load(data);
                            ViewBox.modal("show");
                        }else{
                            SweetAlertPop(Settings.title,a.info,'error');
                        }
                    },
                    fail: function (a) {
                        SweetAlertPop(Settings.title,"An error occurred",'error');
                    },
                    always: function () {
                        setTimeout(()=>{
                            $this.prop("disabled",false);
                        },1000);
                    }
                });
                form.trigger("submit");
            });
        };

        let Edit = ()=>{
            let EditBtn = Staff.find("[app-edit]");
            let EditBox = Staff.find("[app-edit-box]");

            let staffForm = Staff.find("[app-staff-update]");

            let ViewTopImg = EditBox.find("[app-top-view-avatar]");
            let ViewTopName = EditBox.find("[app-top-view-name]");

            let ViewImage = staffForm.find("[app-view-avatar]");
            let ViewFile = staffForm.find("[app-avatar-file]");
            let ViewLoader = staffForm.find("[app-avatar-loader]");

            let ViewID = staffForm.find("[name='id']");
            let ViewUrl = staffForm.find("[name='avatar']");
            let ViewName = staffForm.find("[name='name']");
            let ViewGender = staffForm.find("[name='gender']");
            let ViewMaritalStatus = staffForm.find("[name='maritalStatus']");
            let ViewEmail = staffForm.find("[name='email']");
            let ViewPhone = staffForm.find("[name='phone']");
            let ViewSsn = staffForm.find("[name='ssn']");
            let ViewCountry = staffForm.find("[name='country']");
            let ViewCity = staffForm.find("[name='city']");
            let ViewResidence = staffForm.find("[name='residenceAddress']");
            let ViewUpdate = staffForm.find("[type='submit']");

            let Clear = ()=>{
                ViewImage.attr("src",FileLinks.avatar);
                ViewTopImg.attr("src",FileLinks.avatar);
                ViewName.val("");
                ViewGender.val("");
                ViewMaritalStatus.val("");
                ViewEmail.val("");
                ViewPhone.val("");
                ViewSsn.val("");
                ViewCountry.val("");
                ViewCity.val("");
                ViewResidence.val("");
                ViewUpdate.prop("disabled", false).find(".b-loader").remove();
            };
            let Load = (data)=>{
                let id = data.id;
                let avatar = data.avatar;
                let avatarURL = data.avatarURL;
                let name = data.name;
                let gender = data.gender;
                let maritalStatus = data.maritalStatus;
                let email = data.email;
                let phone = data.phone;
                let ssn = data.ssn;
                let country = data.country;
                let city = data.city;
                let residence = data.residence;

                ViewID.val(id);
                ViewTopName.text(name);
                ViewTopImg.attr("src",avatar);
                ViewImage.attr("src",avatar);
                ViewUrl.val(avatarURL);
                ViewName.val(name);
                ViewGender.val(gender);
                ViewMaritalStatus.val(maritalStatus);
                ViewEmail.val(email);
                ViewPhone.val(phone);
                ViewSsn.val(ssn);
                ViewCountry.val(country);
                ViewCity.val(city);
                ViewResidence.val(residence);
            };
            let UpdateRow = (data)=>{
                let id = data.dataID;
                let name = data.dataName;
                let email = data.dataEmail;
                let phone = data.dataPhone;
                let ssn = data.dataSSN;
                let city = data.city;

                let row = Table.find("[app-staff-id='"+id+"']");
                row.find("[app-update-name]").text(name);
                row.find("[app-update-email]").text(email);
                row.find("[app-update-phone]").text(phone);
                row.find("[app-update-ssn]").text(ssn);
                row.find("[app-update-city]").text(city);
            };

            EditBtn.on("click",function(){
                let $this = $(this);
                let parent = $this.closest("[app-staff-id]");
                let id = parent.attr("app-staff-id");
                let form = $("<form/>");
                form.xsubmit({
                    dataType: 'json',
                    addformData: [
                        ['staff-view', ''],
                        ['id',id]
                    ],
                    before: function () {
                        Clear();
                        $this.prop("disabled",true);
                    },
                    success: function (a) {
                        if(a.found){
                            let data = a.data;
                            Load(data);
                            EditBox.modal("show");
                        }else{
                            SweetAlertPop(Settings.title,a.info,'error');
                        }
                    },
                    fail: function (a) {
                        SweetAlertPop(Settings.title,"An error occurred",'error');
                    },
                    always: function () {
                        setTimeout(()=>{
                            $this.prop("disabled",false);
                        },1000);
                    }
                });
                form.trigger("submit");
            });

            let Update = ()=>{
                let UploadAvatar = ()=>{

                    ViewImage.on("click",()=>ViewFile.trigger("click"));

                    ViewFile.on("change",function(){
                        let file = this.files[0];
                        if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                            $("<form/>").xsubmit({
                                dataType: 'json',
                                addformData: [
                                    ['staff-image', ''],
                                    ['file', file]
                                ],
                                before: function () {
                                    ViewLoader.addClass("show");
                                },
                                success: function (a) {
                                    if(a.success){
                                        let url = a.url;
                                        let uploadurl = a.uploadURL;
                                        ViewImage.attr("src",url);
                                        ViewTopImg.attr("src",url);
                                        ViewUrl.val(uploadurl);
                                    }else{
                                        PNotifyPop(Object.title,a.info,'error');
                                    }
                                },
                                fail: function (a) {
                                    PNotifyPop(Object.title,"An Error Occurred",'error');
                                },
                                always: function () {
                                    setTimeout(function () {
                                        ViewLoader.removeClass("show");
                                    }, 1000);
                                }
                            }).trigger('submit');
                        }else{
                            PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                        }
                    });

                };

                let UpdateProfile = ()=>{
                    staffForm.xsubmit({
                        dataType: 'json',
                        addformData: [
                            ['staff-update', '']
                        ],
                        before: function () {
                            ViewUpdate.prop("disabled",true).append("<i class='b-loader fa fa-spin fa-spinner'></i>");
                            staffForm.find(".form-control").removeClass("form-control-danger");
                        },
                        success: function (a) {
                            if(a.validated){
                                if(a.success){
                                    SweetAlertPop(Settings.title,a.info,'success');
                                    staffForm.trigger("reset");
                                    Clear();
                                    EditBox.modal("hide");
                                    UpdateRow(a);
                                }else{
                                    SweetAlertPop(Settings.title,a.info,'error');
                                }
                            }else{
                                if(a.avatar===false){
                                    PNotifyPop(Settings.appName,a.avatar_t,"error");
                                }
                                if(a.name===false){
                                    ViewName.addClass("form-control-danger");
                                }
                                if(a.gender===false){
                                    ViewGender.addClass("form-control-danger");
                                }
                                if(a.maritalStatus===false){
                                    ViewMaritalStatus.addClass("form-control-danger");
                                }
                                if(a.email===false){
                                    ViewEmail.addClass("form-control-danger");
                                    PNotifyPop(Settings.appName,a.email_t,"error");
                                }
                                if(a.phone===false){
                                    ViewPhone.addClass("form-control-danger");
                                    PNotifyPop(Settings.appName,a.phone_t,"error");
                                }
                                if(a.ssn===false){
                                    ViewSsn.addClass("form-control-danger");
                                    PNotifyPop(Settings.appName,a.ssn_t,"error");
                                }
                                if(a.country===false){
                                    ViewCountry.addClass("form-control-danger");
                                }
                                if(a.city===false){
                                    ViewCity.addClass("form-control-danger");
                                }
                                if(a.residenceAddress===false){
                                    ViewResidence.addClass("form-control-danger");
                                }
                            }
                        },
                        fail: function (a) {
                            SweetAlertPop(Settings.title,"An error occurred",'error');
                        },
                        always: function () {
                            setTimeout(()=>{
                                ViewUpdate.prop("disabled",false).find(".b-loader").remove();
                            },1000);
                        }
                    });
                };

                UploadAvatar();
                UpdateProfile();
            };

            Update();

        };

        let Delete = ()=>{
            let DeleteBtn = Staff.find("[app-delete]");
            DeleteBtn.on("click",function(){
                let $this = $(this);
                let parent = $this.closest("[app-staff-id]");
                let id = parent.attr("app-staff-id");

                SweetAlertQuestion(Settings.appName,"Are you sure you want to remove this staff","question",(res)=>{
                    if(res.value) {
                        let form = $("<form/>");
                        form.xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['staff-delete', ''],
                                ['id', id]
                            ],
                            before: function () {
                                $this.prop("disabled", true).append("<i class='b-loader fa fa-spinner fa-spin'></i>");
                            },
                            success: function (a) {
                                if (a.success) {
                                    parent.remove();
                                }
                            },
                            fail: function (a) {
                                SweetAlertPop(Settings.title, "An error occurred", 'error');
                            },
                            always: function () {
                                setTimeout(() => {
                                    $this.prop("disabled", false).find(".b-loader").remove();
                                }, 1000);
                            }
                        });
                        form.trigger("submit");
                    }
                });
            });
        };

        let Setup = ()=>{
            Table.dataTable();
            View();
            Edit();
            Delete();
        };

        Setup();
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
                $("<form/>").xsubmit({
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
            let branddeleteBtn = Brand.find("[app-delete]");

            branddeleteBtn.on("click",function () {
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

        let modelPreview = Model.find("[model-preview]");
        let modelFile = Model.find("[model-file]");
        let modelLink = Model.find("[model-link]");
        let modelLoader = Model.find("[model-loader]");

        let Previews = Model.find("[model-side-preview]");
        let Handler = Model.find("[model-side-file]");

        let Uploads = ()=>{

            let Banner = ()=>{
                modelPreview.on('click',function () {
                    modelFile.trigger('click');
                });

                modelFile.on("change",function(){
                    let file = this.files[0];
                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {
                                modelLoader.addClass("show");
                            },
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    modelPreview.attr("src",url);
                                    modelLink.val(uploadurl);
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {
                                setTimeout(function () {
                                    modelLoader.removeClass("show");
                                }, 1000);
                            }
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });
            };

            let SideViews = ()=>{

                Previews.on("click",function(){
                    let $this = $(this);
                    let sideFile = $this.siblings("[model-side-file]");
                    sideFile.trigger("click");
                });

                Handler.on("change",function(){
                    let $this = $(this);
                    let file = this.files[0];

                    let sideLoader = $this.siblings("[model-side-loader]");
                    let sidePreview = $this.siblings("[model-side-preview]");
                    let sideUrl = $this.siblings("[model-side-url]");

                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['brand-image', ''],
                                ['file', file]
                            ],
                            before: function () {
                                sideLoader.addClass("show");
                            },
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    sidePreview.attr("src",url);
                                    sideUrl.val(uploadurl);
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {
                                setTimeout(function () {
                                    sideLoader.removeClass("show");
                                }, 1000);
                            }
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }

                });

            };

            Banner();
            SideViews();
        };

        let Feature = ()=> {
            let FeaturesContainer = Model.find("[app-features]");
            FeaturesContainer.on("click","[app-feature-add]",function(){
                let $this = $(this);
                let parent = $this.closest("[app-feature]");
                let copy = parent.html();
                FeaturesContainer.append(`<div class="row m-t-5" app-feature>`+copy+`</div>`);
            });

            let ResetSelect = ()=>{
                FeaturesContainer.children("[app-feature]").not(":first").remove();
                FeaturesContainer.children("[app-feature]").first().find("input").val("");
            };

            return {
                ResetSelect
            }
        };

        let addModel = ()=>{
            let formAdd = Model.find("[app-model-add]");

            let add_submit = formAdd.find("[type='submit']");
            let add_brand = formAdd.find("[name='brand']");
            let add_name = formAdd.find("[name='name']");
            let add_price = formAdd.find("[name='price']");
            let add_description = formAdd.find("[name='description']");
            let add_banner = formAdd.find("[model-fie]");
            let add_side_image = formAdd.find("[model-side-file]");

            let ResetForm = ()=>{
                let sideFront = Model.find("[name='front-url']");
                let sideBack = Model.find("[name='back-url']");
                let sideTop = Model.find("[name='top-url']");
                let sideDown = Model.find("[name='down-url']");
                let sideLeft = Model.find("[name='left-url']");
                let sideRight = Model.find("[name='right-url']");

                modelPreview.attr("src",FileLinks.model);
                sideFront.siblings("[model-side-preview]").attr("src",FileLinks.model_front);
                sideBack.siblings("[model-side-preview]").attr("src",FileLinks.model_back);
                sideTop.siblings("[model-side-preview]").attr("src",FileLinks.model_top);
                sideDown.siblings("[model-side-preview]").attr("src",FileLinks.model_bottom);
                sideLeft.siblings("[model-side-preview]").attr("src",FileLinks.model_left);
                sideRight.siblings("[model-side-preview]").attr("src",FileLinks.model_right);
            };

            formAdd.xsubmit({
                dataType:"json",
                addformData:[
                    ['model_add','']
                ],
                before: function(){
                    add_submit.prop("disabled",true).append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
                },
                success: function (a) {
                    if (a.validated) {
                        if(a.success){
                            PNotifyPop(Settings.appName,a.info,'success');
                            ResetForm();
                            formAdd.trigger("reset");
                            feature.ResetSelect();
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
                        if(a.price===false){
                            add_price.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.price_t, 'error');
                        }
                        if(a.description===false){
                            add_description.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.description_t, 'error');
                        }

                        if(a.banner===false){
                            add_banner.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.banner_t, 'error');
                        }
                        if(a.back===false){
                            add_side_image.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.back_t, 'error');
                        }
                        if(a.down===false){
                            add_side_image.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.down_t, 'error');
                        }
                        if(a.top===false){
                            add_side_image.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.top_t, 'error');
                        }
                        if(a.left===false){
                            add_side_image.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.left_t, 'error');
                        }
                        if(a.right===false){
                            add_side_image.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.right_t, 'error');
                        }
                        if(a.front===false){
                            add_banner.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.front_t, 'error');
                        }
                    }
                },
                fail: function (a) {
                    PNotifyPop(Settings.appName, "An Error Occurred", 'error');
                },
                always:()=>{
                    setTimeout(()=>{
                        add_submit.prop("disabled",false).find(".b-loader").remove();
                    },1000);
                }
            });

        };

        addModel();
        Uploads();
        let feature = Feature();
    };

    let pageModelList = ()=>{
        let Model = pageContent.find("[app-model]");

        let ModelView = ()=>{
            let modelViewBtn = Model.find("[app-view]");
            let ModelViewBox = Model.find("[app-view-box]");

            let ViewBannar = ModelViewBox.find("[app-view-bannar]");
            let ViewSideLeft = ModelViewBox.find("[app-view-sideleft]");
            let ViewSideRight = ModelViewBox.find("[app-view-sideright]");
            let ViewSideFront = ModelViewBox.find("[app-view-sidefront]");
            let ViewSideBack = ModelViewBox.find("[app-view-sideback]");
            let ViewSideTop = ModelViewBox.find("[app-view-sidetop]");
            let ViewSideDown = ModelViewBox.find("[app-view-sidedown]");

            let ViewTitleName = ModelViewBox.find("[app-top-view-name]");
            let ViewTitleLogo = ModelViewBox.find("[app-top-view-brand-logo]");

            let ViewBrand = ModelViewBox.find("[view-brand]");
            let ViewPrice = ModelViewBox.find("[view-price]");
            let ViewModel = ModelViewBox.find("[view-model]");
            let ViewDescription = ModelViewBox.find("[app-view-description]");
            let ViewCreated = ModelViewBox.find("[app-view-created]");
            let ViewUpdated = ModelViewBox.find("[app-view-updated]");
            let ViewFeatures = ModelViewBox.find("[app-features]");

            let ClearData = ()=>{
                ViewBannar.attr("src",FileLinks.brand);
                ViewSideLeft.attr("src",FileLinks.model_left);
                ViewSideRight.attr("src",FileLinks.model_right);
                ViewSideFront.attr("src",FileLinks.model_front);
                ViewSideBack.attr("src",FileLinks.model_back);
                ViewSideTop.attr("src",FileLinks.model_top);
                ViewSideDown.attr("src",FileLinks.model_bottom);
                ViewBrand.text("");
                ViewModel.text("");
                ViewDescription.text("");
                ViewCreated.text("");
                ViewUpdated.text("");
                ViewFeatures.html('');
            };

            let ProcessFeatures = (features)=>{
                features = JSON.parse(features);
                if(features!==null){
                    features.forEach(function (index) {
                        let name = index.name;
                        let value = index.value;
                        ViewFeatures.append(`
                            <tr>
                                <td>`+name+`</td>
                                <td>`+value+`</td>
                            </tr>
                            `);
                    });
                }
            };

            let UpdateData = (data)=>{
                let id = data.id;
                let brand = data.brand;
                let brandName = data.brandName;
                let brandIcon = data.brandIcon;
                let description = data.description;
                let features = data.features;
                let name = data.name;
                let price = data.price;
                let priceText = data.priceText;
                let created = data.created;
                let updated = data.updated;

                let banner = data.banner;
                let sideBack = data.sideBack;
                let sideFront = data.sideFront;
                let sideTop = data.sideTop;
                let sideDown = data.sideDown;
                let sideLeft = data.sideLeft;
                let sideRight = data.sideRight;

                ViewBannar.attr("src",banner);
                ViewSideFront.attr("src",sideFront);
                ViewSideBack.attr("src",sideBack);
                ViewSideTop.attr("src",sideTop);
                ViewSideDown.attr("src",sideDown);
                ViewSideLeft.attr("src",sideLeft);
                ViewSideRight.attr("src",sideRight);

                ViewTitleName.html(brandName+" "+name);
                ViewTitleLogo.attr("src",brandIcon);

                ViewBrand.text(brandName);
                ViewPrice.text(priceText);
                ViewModel.text(name);
                ViewDescription.html(description);
                ViewCreated.text(created);
                ViewUpdated.text(updated);
                ProcessFeatures(features);

            };

            modelViewBtn.on("click", function () {
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
                        ClearData();
                    },
                    success: function(a){
                        setTimeout(()=>{
                            if(a.found) {
                                let data = a.data;
                                UpdateData(data);
                                ModelViewBox.modal('show');
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

        let ModelEdit = ()=>{
            let ModelEditBox = Model.find("[app-edit-box]");
            let ModelLoadEditBtn = Model.find("[app-edit]");
            let updateForm = ModelEditBox.find("[app-model-update]");

            let ViewTopBrandLogo = ModelEditBox.find("[top-view-brand-logo]");
            let ViewTopBrandName = ModelEditBox.find("[top-view-brand-name]");

            let ViewBanner = updateForm.find("[app-view-banner]");
            let ViewSideLeft = updateForm.find("[app-view-sideleft]");
            let ViewSideRight = updateForm.find("[app-view-sideright]");
            let ViewSideFront = updateForm.find("[app-view-sidefront]");
            let ViewSideBack = updateForm.find("[app-view-sideback]");
            let ViewSideTop = updateForm.find("[app-view-sidetop]");
            let ViewSideDown = updateForm.find("[app-view-sidedown]");

            let inputUrl = updateForm.find("[name='url']");
            let inputUrlLeft = updateForm.find("[name='left-url']");
            let inputUrlRight = updateForm.find("[name='right-url']");
            let inputUrlTop = updateForm.find("[name='top-url']");
            let inputUrlDown = updateForm.find("[name='down-url']");
            let inputUrlFront = updateForm.find("[name='front-url']");
            let inputUrlBack = updateForm.find("[name='back-url']");

            let ViewBrand = updateForm.find("[view-brand]");
            let ViewModel = updateForm.find("[view-model]");
            let ViewPrice = updateForm.find("[view-price]");
            let ViewDescription = updateForm.find("[view-description]");
            let ViewFeatures = updateForm.find("[app-features]");
            let modelId = updateForm.find("[app-model-id]");

            let ClearData = ()=>{
                ViewBanner.attr("src",FileLinks.brand);
                ViewSideLeft.attr("src",FileLinks.model_left);
                ViewSideRight.attr("src",FileLinks.model_right);
                ViewSideFront.attr("src",FileLinks.model_front);
                ViewSideBack.attr("src",FileLinks.model_back);
                ViewSideTop.attr("src",FileLinks.model_top);
                ViewSideDown.attr("src",FileLinks.model_bottom);
                ViewBrand.val('');
                ViewModel.val('');
                ViewPrice.val('');
                ViewDescription.val('');
                ViewFeatures.html('');
                modelId.val("");
                ViewTopBrandLogo.attr("src",FileLinks.model);
                ViewTopBrandName.val("");
            };

            let ProcessFeatures = (features)=>{
                features = JSON.parse(features);
                if(features!==null){
                    features.forEach(function (index) {
                        let name = index.name;
                        let value = index.value;
                        ViewFeatures.append(`
                            <div class="row" app-feature>
                                <div class="col-md-4">
                                    <div class="form-group">
                                        <input class="form-control" name="featureName[]" placeholder="Name" value="`+name+`">
                                    </div>
                                </div>
                                <div class="col-md-6">
                                    <div class="form-group">
                                        <input class="form-control" name="featureValue[]" placeholder="Value" value="`+value+`">
                                    </div>
                                </div>
                                <div class="col-md-2">
                                    <button class="btn btn-primary" type="button" app-feature-add>Add</button>
                                </div>
                            </div>
                            `);
                    });
                }
            };

            let FeaturesEvent = ()=>{
                let FeaturesContainer = Model.find("[app-features]");

                FeaturesContainer.on("click","[app-feature-add]",function(){
                    let $this = $(this);
                    let parent = $this.closest("[app-feature]");
                    let copy = parent.html();
                    parent.find("input").val("");
                    FeaturesContainer.append(`<div class="row" app-feature>`+copy+`</div>`);
                });

            };

            let LoadData = (data)=>{
                let id = data.id;
                let brand = data.brand;
                let brandName = data.brandName;
                let brandIcon = data.brandIcon;
                let description = data.description;
                let features = data.features;
                let name = data.name;
                let price = data.price;

                ViewTopBrandLogo.attr("src",brandIcon);
                ViewTopBrandName.text(brandName+" "+name);

                let banner = data.banner;
                let sideBack = data.sideBack;
                let sideFront = data.sideFront;
                let sideTop = data.sideTop;
                let sideDown = data.sideDown;
                let sideLeft = data.sideLeft;
                let sideRight = data.sideRight;

                let bannerRel = data.bannerRel;
                let sideBackRel = data.sideBackRel;
                let sideFrontRel = data.sideFrontRel;
                let sideTopRel = data.sideTopRel;
                let sideDownRel = data.sideDownRel;
                let sideLeftRel = data.sideLeftRel;
                let sideRightRel = data.sideRightRel;

                ViewBanner.attr("src",banner);
                ViewSideFront.attr("src",sideFront);
                ViewSideBack.attr("src",sideBack);
                ViewSideTop.attr("src",sideTop);
                ViewSideDown.attr("src",sideDown);
                ViewSideLeft.attr("src",sideLeft);
                ViewSideRight.attr("src",sideRight);

                inputUrl.val(bannerRel);
                inputUrlFront.val(sideFrontRel);
                inputUrlBack.val(sideBackRel);
                inputUrlTop.val(sideTopRel);
                inputUrlDown.val(sideDownRel);
                inputUrlLeft.val(sideLeftRel);
                inputUrlRight.val(sideRightRel);

                ViewBrand.val(brand);
                ViewModel.val(name);
                ViewPrice.val(price);
                ViewDescription.val(description);
                ProcessFeatures(features);
                modelId.val(id);
            };

            ModelLoadEditBtn.on("click", function () {
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
                            ClearData();
                        },
                        success: function(a){
                            setTimeout(()=>{
                                if(a.found) {
                                    let data = a.data;
                                    LoadData(data);
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

            let UpdateModel = ()=>{
                let update_submit = updateForm.find("[type='submit']");

                let fileBanner = updateForm.find("[model-file]");
                let fileLeftFile = updateForm.find("[model-side-left-file]");
                let fileRightFile = updateForm.find("[model-side-right-file]");
                let fileTopFile = updateForm.find("[model-side-top-file]");
                let fileDownFile = updateForm.find("[model-side-down-file]");
                let fileFrontFile = updateForm.find("[model-side-front-file]");
                let fileBackFile = updateForm.find("[model-side-back-file]");

                ViewBanner.on("click",function(){
                    fileBanner.trigger("click");
                });
                ViewSideLeft.on("click",function(){
                    fileLeftFile.trigger("click");
                });
                ViewSideRight.on("click",function(){
                    fileRightFile.trigger("click");
                });
                ViewSideTop.on("click",function(){
                    fileTopFile.trigger("click");
                });
                ViewSideDown.on("click",function(){
                    fileDownFile.trigger("click");
                });
                ViewSideFront.on("click",function(){
                    fileFrontFile.trigger("click");
                });
                ViewSideBack.on("click",function(){
                    fileBackFile.trigger("click");
                });


                fileBanner.on("change",function(){
                    let $this = $(this);
                    let modelLoader = $this.siblings("[model-loader]");
                    let file = this.files[0];
                    if (file && $.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {
                                modelLoader.addClass("show");
                            },
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    ViewBanner.attr("src",url);
                                    inputUrl.val(uploadurl);
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {
                                setTimeout(function () {
                                    modelLoader.removeClass("show");
                                }, 1000);
                            }
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });
                fileLeftFile.on("change",function(){
                    let file = this.files[0];
                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {},
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    ViewSideLeft.attr("src",url);
                                    inputUrlLeft.val(uploadurl);
                                    fileLeftFile.val('');
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {}
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });
                fileRightFile.on("change",function(){
                    let file = this.files[0];
                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {},
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    ViewSideRight.attr("src",url);
                                    inputUrlRight.val(uploadurl);
                                    fileRightFile.val('');
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {}
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });
                fileTopFile.on("change",function(){
                    let file = this.files[0];
                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {},
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    ViewSideTop.attr("src",url);
                                    inputUrlTop.val(uploadurl);
                                    fileTopFile.val('');
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {}
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });
                fileDownFile.on("change",function(){
                    let file = this.files[0];
                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {},
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    ViewSideDown.attr("src",url);
                                    inputUrlDown.val(uploadurl);
                                    fileDownFile.val('');
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {}
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });
                fileFrontFile.on("change",function(){
                    let file = this.files[0];
                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {},
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    ViewSideFront.attr("src",url);
                                    inputUrlFront.val(uploadurl);
                                    fileFrontFile.val('');
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {}
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });
                fileBackFile.on("change",function(){
                    let file = this.files[0];
                    if ($.inArray(file.type, Settings.AcceptedImages.imageArray()) >= 0) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['model-image', ''],
                                ['file', file]
                            ],
                            before: function () {},
                            success: function (a) {
                                if(a.success){
                                    let url = a.url;
                                    let uploadurl = a.uploadURL;
                                    ViewSideBack.attr("src",url);
                                    inputUrlBack.val(uploadurl);
                                    fileBackFile.val('');
                                }else{
                                    PNotifyPop(Object.title,a.info,'error');
                                }
                            },
                            fail: function (a) {
                                PNotifyPop(Object.title,"An Error Occurred",'error');
                            },
                            always: function () {}
                        }).trigger('submit');
                    }else{
                        PNotifyPop(Object.title,"Selected file is not a Picture.",'error');
                    }
                });

                updateForm.xsubmit({
                    dataType:"json",
                    addformData:[
                        ['model-update',''],
                    ],
                    before: function(){
                        update_submit.append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
                    },
                    success: function (a) {
                        if(a.success){
                            PNotifyPop(Settings.appName,a.info,'success');
                            updateForm.trigger("reset");
                            let row = Model.find("[app-model-id='"+a.dataID+"']");
                            row.find("[app-update-name]").text(a.dataName);
                            row.find("[app-update-price]").text(a.dataPrice);
                            row.find("[app-update-brand]").text(a.dataBrand);
                            row.find("[app-updated]").text(a.dataUpdate);

                            ModelEditBox.modal("hide");
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

            FeaturesEvent();
            UpdateModel();
        };

        let ModelDelete = ()=>{
            let deleteBtn = Model.find("[app-delete]");
            deleteBtn.on("click",function () {
                let $this = $(this);
                let row = $this.closest("[app-model-id]");
                let id = row.attr("app-model-id");
                    SweetAlertQuestion(Settings.appName,"Are you sure want to delete?","question",(res)=>{
                        if (res.value) {
                            $("<form/>").xsubmit({
                                dataType: 'json',
                                addformData: [
                                    ['delete-Model', ''],
                                    ['id', id]
                                ],
                                success: (a) => {
                                    if (a.success) {
                                        swalWithBootstrapButtons.fire(
                                            'Deleted!',
                                            'Model deleted.',
                                            'success'
                                        );
                                        Model.find("[app-model-id='"+id+"']").remove();

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

        let Setup = ()=>{

        };

        Setup();
        ModelView();
        ModelEdit();
        ModelDelete();
    };

    /***JOBS PAGE =================***/
    let pageJobsAdd = ()=>{
        let Jobs = pageContent.find("[app-news]");
        let PostJob = Jobs.find("[app-post-btn]");

        CKEDITOR.replace('editor1',{
            height:380,
            extraPlugins: 'simage',
            imageUploadURL: 'server?file-image',
            dataParser: function(data){
                //console.log(data);
                return (data.url);
            },
            srcSet:function(data){
               return(data.url);
            }

        });

        PostJob.on('click',function () {
            let form_add = Jobs.find("[app-jobs-add]");

            let add_button = form_add.find("[type='button']");
            let add_title = form_add.find("[name='title']");
            let add_body = form_add.find("[name='body']");
            let iframe = document.querySelector("iframe");
            let constantBody= iframe.contentWindow.document.getElementsByTagName("body")[0].innerHTML;


            $("<form/>").xsubmit({
                dataType:"json",
                addformData:[
                    ['jobs_add',''],
                    ['title',add_title.val()],
                    ['body',constantBody],
                ],
                before: function(){
                    add_button.append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
                },
                success: function (a) {
                    if (a.validated) {
                        if(a.success){
                            PNotifyPop(Settings.appName,a.info,'success');
                            form_add.trigger("reset");
                            add_body.innerHTML = "";
                        }else{
                            PNotifyPop(Settings.appName,a.info,'error');
                        }
                    } else {
                        if(a.title===false){
                            add_title.parent().addClass('has-error');
                            PNotifyPop(Settings.appName, a.title_t,'error');
                        }
                    }
                }, fail: function (a) {
                    PNotifyPop(Settings.appName, "An Error Occurred", 'error');
                },always:()=>{
                    setTimeout(()=>{
                        add_button.find(".b-loader").remove();
                    },1000);
                }
            }).trigger("submit");
        });
    };

    let pageJobsList = ()=>{
        let Jobs = pageContent.find("[app-news-list]");
        let JobEditBox = Jobs.find("[app-edit]");

        CKEDITOR.replace('editor1',{
            height:380,
            extraPlugins: 'simage',
            imageUploadURL: 'server?file-image',
            dataParser: function(data){
                //console.log(data);
                return (data.url);
            },
            srcSet:function(data){
                return(data.url);
            }

        });

        let JobEdit = ()=>{
            let jobEditBtn = Jobs.find("[app-edit-news]");

            jobEditBtn.on("click", function () {
               JobEditBox.modal("show");
            });

        }

        let JobDelete = ()=>{
            let deleteBtn = Jobs.find("[app-news-delete]");
            deleteBtn.on("click",function () {
                let $this = $(this);
                let row = $this.closest("[app-job-id]");
                let id = row.attr("app-job-id");
                SweetAlertQuestion(Settings.appName,"Are you sure want to delete?","question",(res)=>{
                    if (res.value) {
                        $("<form/>").xsubmit({
                            dataType: 'json',
                            addformData: [
                                ['delete-job', ''],
                                ['id', id]
                            ],
                            success: (a) => {
                                if (a.success) {
                                    swalWithBootstrapButtons.fire(
                                        'Deleted!',
                                        'Job deleted.',
                                        'success'
                                    );
                                    Jobs.find("[app-job-id='"+id+"']").remove();

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
        }

        JobEdit();
        JobDelete();
    };


    /***PASSWORD PAGE ===============***/
    let pagePassword = ()=>{
        let Password = pageContent.find("[app-password]");

        let Reset = ()=>{
            let form_reset = Password.find("[app-password-reset]");

            let submit_form = form_reset.find("[type='submit']");
            let oldpass = form_reset.find("[name='oldpassword']");
            let newpass = form_reset.find("[name='newpassword']");
            let repeatpass = form_reset.find("[name='repeatpassword']");

            form_reset.xsubmit({
                dataType:"json",
                addformData:[
                    ['update-password','']
                ],
                before: function(){
                    form_reset.append("<i class='b-loader fa fa-spin fa-spinner m-l-10'></i>");
                },
                success: function (a) {
                        if(a.success){
                            PNotifyPop(Settings.appName,a.info,'success');
                            form_reset.trigger("reset");
                        }else{
                            PNotifyPop(Settings.appName,a.info,'error');
                        }
                }, fail: function (a) {
                    PNotifyPop(Settings.appName, "An Error Occurred", 'error');
                },always:()=>{
                    setTimeout(()=>{
                        form_reset.find(".b-loader").remove();
                    },1000);
                }
            });

        };

        Reset();
    };


    /***CUSTOMER PAGE ===============***/
    let pageCustomer = ()=>{

    };

    /***PAGES ACTION END ============================***/


    General();



});