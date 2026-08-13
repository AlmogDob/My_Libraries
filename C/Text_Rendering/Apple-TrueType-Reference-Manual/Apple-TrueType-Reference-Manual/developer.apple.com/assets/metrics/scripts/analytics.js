if (document.location.search && s_account) {
    var dls = document.location.search;
    if (dls.indexOf("?cid=AOS-") > -1 || dls.indexOf("&cid=AOS-") > -1) {
        s_account += ",applestoreWW"
    }
}(function() {
    function getStoreBucket() {
        var metas = document.getElementsByTagName("meta");
        var metaLength = metas.length;
        for (i = 0; i < metaLength; i++) {
            if (metas[i].getAttribute("property") === "analytics-s-bucket-store") {
                return metas[i].getAttribute("content")
            }
        }
        return ""
    }
    var storeBucket = getStoreBucket();
    if (storeBucket) {
        s_account += "," + storeBucket
    }
})();
var s = s_gi(s_account);

if (navigator && navigator.loadPurpose && navigator.loadPurpose == "preview") {
    s.t = new Function("return ''")
}

s._isSafari = false;
if (navigator.userAgent.toLowerCase().indexOf("webkit") > -1) {
    if (navigator.userAgent.toLowerCase().indexOf("safari") > -1 && navigator.userAgent.toLowerCase().indexOf("chrome") < 0) {
        s._isSafari = true
    }
}

function safariHandler(link) {
    if (s.lt(link.href)) {
        link.addEventListener("mouseup", function(evt) {
            if (evt.which && evt.which == 1 || evt.button && evt.button == 1) {
                var linkHref = evt.currentTarget.href,
                    linkType = s.lt(linkHref);
                if (linkType == "d") {
                    if (linkHref.match(/\.rss|\.xml/)) {
                        s.eVar16 = s.prop16 = "sign ups"
                    } else {
                        s.eVar11 = AC.Tracking.pageName() + " - " + linkHref.substring(linkHref.lastIndexOf("/") + 1, linkHref.length);
                        s.eVar11 = s.eVar11.toLowerCase();
                        s.eVar16 = s.prop16 = "Downloads";
                        // s.events = s.apl(s.events, "event5", ",", 1)
                    }
                    s.linkTrackVars = "prop16,eVar16,eVar11,events";
                    s.linkTrackEvents = "event5"
                }
                s.linkTrackVars = "None";
                s.linkTrackEvents = "None"
            }
        }, false)
    }
}

s.currencyCode = "USD";
if (window.location.hostname.indexOf(".com.cn") > -1) {
    s.fpCookieDomainPeriods = "3"
}
s.trackDownloadLinks = true;
s.trackExternalLinks = true;
s.trackInlineStats = true;
s.useForcedLinkTracking = true;
s.forcedLinkTrackingTimeout = 100;
s.linkDownloadFileTypes = "avi,dmg,doc,docx,exe,extz,gz,m4a,mov,mp3,mpg,pdf,pkg,ppt,pptx,rss,safariextz,sit,wav,wmv,xls,xlsx,xml,zip";
s.linkInternalFilters = "javascript:,apple.com,mailto:,tel:";
s.linkLeaveQueryString = false;
s.linkTrackVars = "campaign";
s.linkTrackEvents = "None";
s.loadModule("Media");
s.Media.autoTrack = false;
s.Media.trackVars = "prop13,hier1";
s.Media.trackEvents = "None";
s.Media.trackWhilePlaying = true;
s.Media.trackMilestones = "10,50,90";
s.Media.monitor = function(s, media) {
    if (media.event == "CLOSE") {
        if (media.percent >= "99") {
            s.Media.trackVars = "prop13,prop16,eVar16";
            s.prop13 = "v@e: " + media.name;
            s.eVar16 = s.prop16 = "video ends";
            s.Media.track(media.name);
            s.prop13 = s.prop16 = s.eVar16 = ""
        }
        if (media.percent < "99") {
            var tmpEvent = s.events,
                tmpProp13 = s.prop13,
                tmpProp16 = s.prop16,
                tmpProp4 = s.prop4,
                tmpTrackVars = s.Media.trackVars,
                tmpTrackEvents = s.Media.trackEvents;
            s.events = s.prop13 = s.prop16 = s.eVar16 = s.prop4 = "";
            s.Media.trackVars = s.Media.trackEvents = "";
            s.Media.track(media.name);
            s.events = tmpEvent;
            s.prop13 = tmpProp13;
            s.prop16 = s.eVar16 = tmpProp16;
            s.prop4 = tmpProp4;
            s.Media.trackVars = tmpTrackVars;
            s.Media.trackEvents = tmpTrackEvents
        }
    }
};
s.eVar54 = document.location.href;
s.eVar49 = document.referrer;


var s_vi_vnum = s.c_r("s_vnum_n2_us");
if (s_vi_vnum) {
    var date = new Date;
    date.setTime(date.getTime() + 63072e6);
    var expires = "; expires=" + date.toGMTString();
    document.cookie = "s_vnum_n2_us=" + s_vi_vnum + expires + "; domain=apple.com; secure; path=/"
}
var s_vi = s.c_r("s_vi");
if (s_vi) {
    var date = new Date;
    date.setTime(date.getTime() + 63072e6);
    var expires = "; expires=" + date.toGMTString();
    document.cookie = "s_vi=" + s_vi + expires + "; domain=apple.com; secure; path=/"
}
var s_pv = s.c_r("s_pv");
if (s_pv) {
    var expires = ";expires=Thu, 01 Jan 1970 00:00:01 GMT";
    document.cookie = "s_pv=" + s_pv + expires + "; domain=apple.com; secure; path=/"
}

function QTCheck() {
    if (AC && typeof AC.Detector != "undefined" && typeof AC.Detector.getQTVersion != "undefined") {
        return AC.Detector.isMobile() || AC.Detector.isiPad() ? "mobile" : AC.Detector.getQTVersion() == "0" ? "no quicktime" : "quicktime " + AC.Detector.getQTVersion().split(/\./)[0] + ".x"
    }
    return "quicktime not detected"
}
if (typeof iTunesDetected === "function") {
    var activeX = document.createElement("object");
    activeX.setAttribute("width", 1);
    activeX.setAttribute("height", 1);
    activeX.id = "iTunesDetectorIE";
    activeX.setAttribute("classid", "clsid:D719897A-B07A-4C0C-AEA9-9B663A28DFCB");
    document.getElementsByTagName("head")[0].appendChild(activeX);
    s.prop12 = iTunesDetected() ? "itunes" : "no itunes"
}
if (typeof AC == "undefined") {
    AC = {}
}
if (!AC.Tracking) {
    AC.Tracking = {}
}
AC.Tracking._pageName = null;
AC.Tracking.pageName = function() {
    if (AC.Tracking._pageName) {
        return AC.Tracking._pageName
    }
    var metaTags = document.getElementsByTagName("meta");
    for (var i = 0, metaTag; metaTag = metaTags[i]; i++) {
        if ("omni_page" === metaTag.getAttribute("name")) {
            AC.Tracking._pageName = metaTag.getAttribute("content").toLowerCase();
            return AC.Tracking._pageName
        }
    }
    return AC.Tracking._pageNameForTitle_atHost_andPath(document.title, window.location.hostname, window.location.pathname)
};
AC.Tracking._pageNameForTitle_atHost_andPath = function(title, host, pathName) {
    var pageName = title.toLowerCase();
    if (/\s-\s/.test(pageName)) {
        pageName = pageName.replace(/\s*-?\s*(apple|ã‚¢ãƒƒãƒ—ãƒ«|ì• í”Œì»´í“¨í„°ì½”ë¦¬ì•„|è˜‹æžœ|è˜‹æžœé›»è…¦|appleä¸­å›½|è‹¹æžœä¸­å›½)\s+[^-]*-?\s*/, "")
    }
    if (!pathName.match(/^\/(ws|pr|g5|go|ta|wm)\//)) {
        pathName = pathName.replace(/^\/(\w{2}|befr|benl|chfr|chde|asia|lae)(?=\/)/, "")
    }
    if (pathName.match(/\//g).length <= 2 && !pathName.match(/support/) && !host.match(/support/) && !host.match(/selfsolve/) && (!!pathName.match(/index\.html/) || !pathName.match(/\.html/))) {
        pageName += " - index"
    }
    if (/\/pr\//.test(pathName)) {
        pageName = "pr - " + pageName
    }
    return pageName
};

s.usePlugins = true;

function s_doPlugins(s) {
    s.tcall = typeof s.linkType == "undefined" ? true : false;
    if (s.pageName) {
        var pgEscaped = escape(s.pageName);
        pgEscaped = pgEscaped.replace(/(%u2018|%u2019|%u02BC|%u02BD)/g, "%27");
        pgEscaped = pgEscaped.replace(/(%u201C|%u201D|%E2%80%9C|%E2%80%9D)/g, "%22");
        pgEscaped = pgEscaped.replace(/(%09|%0A|%0D)/g, "");
        s.pageName = unescape(pgEscaped)
    }
    
    if (window.devicePixelRatio >= 2) {
        s.prop5 = navigator.platform + " 2x"
    } else {
        s.prop5 = navigator.platform
    }
    
    tempVar1 = s.Util.getQueryParam("ref");
    if (tempVar1 && s.tcall) s.referrer = tempVar1;
    else if (tempVar1 && !s.tcall) s.referrer = "";
    s.server = s.Util.getQueryParam("alias");
    if (!s.server) {
        s.server = "new approach legacy"
    }
    if (!s.campaign) {
        s.campaign = s.Util.getQueryParam("cid");
        //s.setClickMapEmail("Email_PageName,Email_OID", "Email_OT");
        if (s.campaign.match(/OAS-.+?-DOMAINS-/i)) {
            var tempVar0 = "http://" + s.campaign.replace(/OAS-.+?-DOMAINS-/i, "");
            s.referrer = s.tcall ? tempVar0 : ""
        }
    }
    s.campaign = s.getValOnce(s.campaign, "s_campaign", 0);
    s.prop6 = !s.prop6 ? 'D="' + s.Util.getQueryParam("cp").toLowerCase() + ': "+pageName' : s.prop6;
    s.prop11 = s.Util.getQueryParam("sr");
    if (!s.d.URL.match(/\/channel\//) && !s.prop11 && s.c_r("s_3p")) {
        s.prop11 = s.c_r("s_3p");
        s.c_w("s_3p", "", -1)
    }
    s.eVar7 = !s.eVar7 ? s.Util.getQueryParam("aid") : "";
    s.eVar7 = s.getValOnce(s.eVar7, "s_var_7", 0);
 
    // if (s.eVar2) s.events = s.apl(s.events, "event6", ",", 1);
    if (!s.d.URL.match(/apple.com\/(\w{2}|befr|benl|chfr|chde|asia|lae)\/search\//) && !s.d.URL.match(/apple.com\/search\//) && (s.d.referrer.match(/apple.com\/(\w{2}|befr|benl|chfr|chde|asia|lae)\/search\//) || s.d.referrer.match(/apple.com\/search\//))) {
        s.eVar2 = s.d.referrer.match(/\/support\//) ? "acs: " : s.d.referrer.match(/\/store\//) ? "aos: " : "www: ";
        if (s.d.referrer.match(/apple.com\/(\w{2}|befr|benl|chfr|chde|asia|lae)\/search/)) {
            s.eVar2 += s.getQueryParam("q", "", s.d.referrer).replace(/\+/g, " ");
            var geo = s.d.referrer.match(/\/(\w{2}|befr|benl|chfr|chde|asia|lae)\//i);
            s.eVar2 += " (" + geo[0].replace(/\//g, "") + ")"
        } else {
            s.eVar2 += s.getQueryParam("q", "", s.d.referrer).replace(/\+/g, " ") + " (us)"
        }
    }
    if (s.prop11 == "em" && s.tcall) {
        s.referrer = "imap://chatterbox.com"
    }
    if (s.prop11 == "app" && s.tcall) {
        s.referrer = "file://fromApp"
    }
    if (document.referrer && document.referrer.indexOf("apple.com/startpage/") > -1 && s.tcall) {
        s.referrer = "news://startpage.com";
        s._1_referrer = 1
    }
    if (!s.prop17) {
        var percent = s.getPercentPageViewed();
        if (percent && percent.length >= 4 && typeof percent[1] != "undefined") {
            s.prop14 = percent[0];
            s.prop17 = percent[1] + ":" + percent[2];
            s.prop28 = Math.round(percent[3] / 10) * 10;
            s.eVar17 = s.eVar18 = "";
            if (percent[4]) {
                var sva = percent[4].split(/\|/g),
                    sv = "";
                for (var i = 0; i < sva.length; i++) {
                    if (i != sva.length - 1) {
                        var diff = sva[i + 1].split(/:/)[0] - sva[i].split(/:/)[0];
                        if (diff > 100) {
                            sv += sva[i].split(/:/)[1];
                            var nz = diff / 100;
                            while (nz > 1) {
                                sv += "0";
                                nz--
                            }
                        } else {
                            sv += sva[i].split(/:/)[1]
                        }
                    } else {
                        sv += sva[i].split(/:/)[1]
                    }
                }
                if (sv.length > 254) {
                    s.eVar17 = sv.substring(0, 254);
                    s.eVar18 = sv.substring(255, sv.length)
                } else {
                    s.eVar17 = sv
                }
            }
            if (!s.tcall) {
                s.linkTrackVars = "prop17,prop28"
            }
        }
    }
    s.prop32 = s.eVar32 = s.Util.getQueryParam("psid");
    if (s.prop32 || s.c_r("s_sid")) {
        var e = new Date,
            ct = e.getTime();
        e.setTime(ct + 63072e4);
        s.prop32 ? s.c_w("s_psid", s.prop32, e) : s.c_w("s_psid", s.c_r("s_sid"), e);
        s.c_w("s_sid", "", -1)
    }
    if (!s.prop32 && !s.c_r("s_pathLength")) {
        s.prop32 = s.c_r("s_psid")
    }
    s.linkLeaveQueryString = true;
    // var url = s.downloadLinkHandler();
    // if (url) {
    //     if (url.match(/\.rss|\.xml/)) {
    //         s.eVar16 = s.prop16 = "sign ups"
    //     } else {
    //         s.eVar11 = AC.Tracking.pageName() + " - " + url.substring(url.lastIndexOf("/") + 1, url.length);
    //         s.eVar16 = s.prop16 = "downloads";
    //         s.events = s.apl(s.events, "event5", ",", 1)
    //     }
    //     s.linkTrackVars = "prop16,eVar16,eVar11,events";
    //     s.linkTrackEvents = "event5"
    // }
    s.linkLeaveQueryString = false;
    if (typeof Media != "undefined" && s.tcall) {
        s.prop18 = QTCheck()
    }
    
            function osDetect(s) {
            var userAgent = navigator.userAgent;
            var match;
            if (userAgent.match(/windows/i)) {
                s.prop9 = "windows";
                return
            }
            if (userAgent.match(/(kindle|silk-accelerated)/i)) {
                if (userAgent.match(/(kindle fire|silk-accelerated)/i)) {
                    s.prop9 = "kindle fire"
                } else {
                    s.prop9 = "kindle"
                }
                return
            }
            if (userAgent.match(/(iphone|ipod|ipad)/i)) {
                match = userAgent.match(/OS [0-9_]+/i);
                s.prop9 = "i" + match[0].replace(/_/g, ".");
                return
            }
            if (userAgent.match(/android/i)) {
                s.prop9 = userAgent.match(/android [0-9]\.?[0-9]?\.?[0-9]?/i);
                return
            }
            if (userAgent.match(/webos\/[0-9\.]+/i)) {
                match = userAgent.match(/webos\/[0-9]\.?[0-9]?\.?[0-9]?/i);
                s.prop9 = match[0].replace(/webos\//i, "web os ");
                return
            }
            if (userAgent.match(/rim tablet os [0-9\.]+/i)) {
                match = userAgent.match(/rim tablet os [0-9]\.?[0-9]?\.?[0-9]?/i);
                s.prop9 = match[0].replace(/rim tablet os/i, "rim os ");
                return
            }
            if ((userAgent.match(/firefox\/(\d{2}||[3-9])/i) || userAgent.match(/AppleWebKit\//)) && userAgent.match(/Mac OS X [0-9_\.]+/)) {
                var matches = userAgent.match(/[0-9_\.]+/g);
                matches = matches[1].split(/_|\./);
                s.prop9 = matches[0] + "." + matches[1] + ".x";
                return
            }
            var mv = userAgent.match(/AppleWebKit\/\d*/i) && userAgent.match(/AppleWebKit\/\d*/i).toString().replace(/AppleWebKit\//i, "");
            if (mv > 522) {
                s.prop9 = "10.5.x"
            } else if (mv > 400) {
                s.prop9 = "10.4.x"
            } else if (mv > 99) {
                s.prop9 = "10.3.x"
            } else if (mv > 80) {
                s.prop9 = "10.2.x"
            } else {
                s.prop9 = "mac unknown or non-safari"
            }
        }
    
    osDetect(s);
    if (s.pageName && s.pageName.match(/feedback - thank you/)) {
        s.prop16 = s.eVar16 = "feedback"
    }
    if (s.prop13 && (s.tcall || s.linkType == "o" || s.linkType == "")) {
        if (s.pageName && !s.pageName.match(/movie trailers -/)) {
            if (s.prop13.match(/(v@s|v@r)/i)) {
                s.prop16 = s.eVar16 = "video plays";
                s.events = "event2";
                if (!s.tcall) {
                    s.linkTrackEvents += ",event2";
                    s.linkTrackVars += ",events,prop16,eVar16"
                }
            }
            if (s.prop13.match(/v@e/i)) {
                s.prop16 = s.eVar16 = "video ends";
                if (!s.tcall) {
                    s.linkTrackEvents = "";
                    s.linkTrackVars += ",prop16,eVar16"
                }
            }
        }
    }
    s.linkLeaveQueryString = true;
    // var exitUrl = s.linkHandler("itms.apple.com|itunes.apple.com|apps.apple.com", "e");
    // var url = s.linkHandler("ax.itunes.apple.com/WebObjects/MZStoreServices.woa/ws/RSS/|rss.support.apple.com", "o");
    // if (url) {
    //     s.eVar16 = s.prop16 = "sign ups";
    //     s.linkTrackVars = "eVar16,prop16"
    // }
    s.linkLeaveQueryString = false;
    if (s.tcall) {
        var membership, pathname = window.location.pathname,
            newMembership = false,
            noMatch = true;
        if (s.c_r("iTunesPresent") || s.prop12 && s.prop12 == "iTunes") {
            membership = membership ? membership + "it," : "it,"
        }
        if (s.c_r("hasMobileMe")) {
            membership = membership ? membership + "mm," : "mm,"
        }
        if (s.c_r("DefaultAppleID") || s.pageName && s.pageName.match(/iforgot - cr or email option/)) {
            membership = membership ? membership + "aid," : "aid,"
        }
        if (s.c_r("trackStartpage")) {
            membership = membership ? membership + "sp," : "sp,"
        }
        if (s.prop11) {
            if (s.prop11.match("3p")) {
                membership = membership ? membership + "3p," : "3p,"
            }
        }
        if (s.pageName) {
            if (s.pageName.match(/one to one - index/)) {
                membership = membership ? membership + "o2o," : "o2o,"
            }
        }
        if (pathname.match("/welcomescreen/")) {
            var portion;
            if (portion = pathname.match("ilife.*")) {
                portion = "il" + portion.toString().match("[0-9]+") + ",";
                membership = membership ? membership + portion : portion
            } else if (portion = pathname.match("iwork.*")) {
                portion = "iwk" + portion.toString().match("[0-9]+") + ",";
                membership = membership ? membership + portion : portion
            } else if (portion = pathname.match("itunes.*")) {
                portion = "it" + portion.toString().match("[0-9]+") + ",";
                membership = membership ? membership + portion : portion
            } else if (portion = pathname.match("aperture.*")) {
                portion = "ap" + portion.toString().match("[0-9]+") + ",";
                membership = membership ? membership + portion : portion
            }
        }
        if (s.Util.getQueryParam("sr") && s.Util.getQueryParam("vr")) {
            var ver = s.Util.getQueryParam("vr");
            ver = ver.substring(0, ver.indexOf("-")) + ",";
            membership = membership ? membership + ver : ver
        }
        if (typeof membership != "undefined") {
            membership = membership.substring(0, membership.length - 1).toLowerCase();
            membership = membership.split(",");
            if (s.c_r("s_membership")) {
                var membershipCookie = s.c_r("s_membership").split(/:/);
                membershipCookie.splice(0, 1);
                for (i = 0; i < membership.length; i++) {
                    for (j = 0; j < membershipCookie.length; j++) {
                        if (membershipCookie[j] == membership[i]) {
                            noMatch = false
                        }
                    }
                    if (noMatch) {
                        membershipCookie[membershipCookie.length] = membership[i];
                        newMembership = true
                    }
                    noMatch = true
                }
                if (newMembership) {
                    membership = membershipCookie.length + ":" + membershipCookie.toString().replace(/,/g, ":");
                    var e = new Date,
                        ct = e.getTime();
                    e.setTime(ct + 63072e3);
                    s.c_w("s_membership", membership, e);
                    s.prop31 = membership
                }
            } else {
                membership = membership.length + ":" + membership.toString().replace(/,/g, ":");
                var e = new Date,
                    ct = e.getTime();
                e.setTime(ct + 63072e3);
                s.c_w("s_membership", membership, e);
                s.prop31 = membership
            }
        }
        if (!s.prop31 && !s.c_r("s_pathLength")) {
            s.prop31 = s.c_r("s_membership")
        }
    }(function getNavigationSource() {
        function getEntryPoint() {
            var currentDomain = window.location.host;
            var referrer = document.referrer;
            var referrerFriendlyName = "";
            if (!referrer) {
                referrerFriendlyName = "direct entry"
            } else if (referrer.split("?")[0].indexOf(currentDomain) === -1) {
                referrerFriendlyName = "third party"
            }
            return referrerFriendlyName
        }
        var entryPoint = getEntryPoint();
        if (entryPoint && entryPoint !== "") {
            s.prop25 = entryPoint
        }
        if (s.tcall && !s.prop25) {
            var navSource;
            var isStoreLink=false;
            if (isStoreLink === true) {
                if (window.localStorage !== undefined) {
                    navSource = window.localStorage.getItem("apple_Metrics");
                    window.localStorage.removeItem("apple_Metrics")
                }
            } else {
                if (AC && typeof AC.Storage === "object") {
                    navSource = AC.Storage.getItem("ac-storage-apple_Metrics");
                    AC.Storage.removeItem("ac-storage-apple_Metrics")
                }
            }
            if (navSource) {
                var navData;
                try {
                    navData = JSON.parse(navSource)
                } catch (e) {}
                s.prop25 = navData.region ? navData.region : null;
                s.eVar1 = navData.eVar1 ? navData.eVar1 : null;
                if (navData.pageName) {
                    s.prop14 = navData.pageName
                }
                if (navData.events) {
                    if (!s.events) {
                        s.events = navData.events
                    } else {
                        s.events += "," + navData.events
                    }
                }
            }
        }
        if (s.tcall) {
            if (document.referrer.match(/(downloads|epp|store|storeint)\.apple\.com/)) {
                s.prop25 = "aos nav"
            }
        }
        if (!s.prop25) {
            s.prop25 = "other nav or none"
        }
    })();
    if ((s.pageName && s.prop14 && s.pageName.toLowerCase() != s.prop14.toLowerCase() || !s.prop14) && s.tcall) {
        var ch, cookieValue = s.c_r("s_pathLength"),
            pathLengthArray = cookieValue.indexOf(",") > -1 ? cookieValue.split(",") : [],
            e = new Date,
            ct = e.getTime();
        e.setTime(ct + 30 * 60 * 1e3);
        if (s.channel) {
            ch = s.channel.substring(s.channel.indexOf(".") + 1, s.channel.length);
            ch = ch.substring(ch.indexOf(".") + 1, ch.length)
        } else {
            ch = "no channel"
        }
        if (pathLengthArray.length != 0 && pathLengthArray.toString().indexOf(ch + "=") > -1) {
            for (i = 0; i < pathLengthArray.length; i++) {
                if (pathLengthArray[i].toString().indexOf(ch + "=") > -1) {
                    var pathLengthValue = pathLengthArray[i].split("=");
                    ++pathLengthValue[1];
                    pathLengthArray[i] = pathLengthValue[0] + "=" + pathLengthValue[1];
                    s.prop48 = pathLengthValue[1]
                }
            }
            s.c_w("s_pathLength", pathLengthArray, e)
        } else {
            var pathLengthValue = cookieValue + ch + "=" + 1 + ",";
            s.c_w("s_pathLength", pathLengthValue, e);
            s.prop48 = "1"
        }
    }

    var eVar10 = s.Util.getQueryParam ? s.Util.getQueryParam("afid") : null;
    if (eVar10) {
        s.eVar10 = s.getValOnce ? s.getValOnce(eVar10, "s_afc") : null
    }(function() {
        var topEl = document.getElementById("top");
        var locale = topEl ? topEl.getAttribute("data-analytics-locale") : "";
        if (locale) {
            s.prop20 = "aos: " + locale;
            s.prop19 = s.prop20 + (s.pageName ? ": " + s.pageName : "");
            s.eVar3 = s.prop20 || ""
        }
    })();
    (function() {
        if (s.tcall) {
            s.eVar147 = s.c_r('s_ev147');
        } else {
            var acctNode = document.getElementById('globalnav-account');
            var val147 = (acctNode && acctNode.classList.contains('authenticated')) ? 'authenticated' : 'unauthenticated';
            s.eVar147 = val147;
            s.c_w('s_ev147', val147);
        }

        if (s.tcall) {
            s.eVar148 = s.c_r('s_ev148');
        } else {
            var node = s.clickObject;
            var vals148 = [];
            while (node && node !== document) {
                if (node.getAttribute && node.getAttribute('data-aa-content-module')) {
                    vals148.push(node.getAttribute('data-aa-content-module'));
                }
                node = node.parentElement || node.parentNode;
            }
            s.eVar148 = vals148.length ? vals148.reverse().join('|') : 'static';
            s.c_w('s_ev148', s.eVar148);
        }
    })();
    s.hier1 = s.channel ? s.channel : "";
    // s.linkTrackVars = s.apl(s.linkTrackVars, "hier1", ",", 1);
    s.prop4 = s.prop4 ? s.prop4 : "D=g";
    //s.manageVars("lowercaseVars", "purchaseID,pageType,events,products,transactionID", 2)
}
s.doPlugins = s_doPlugins;

s.getPercentPageViewed = function() {
    if ("undefined" == typeof s.linkType) return s.ppv.previous = sessionStorage.getItem(s.ppv.sessionStorageKey) ? sessionStorage.getItem(s.ppv.sessionStorageKey) : "", s.ppv.init(), s.ppv.previous.split(",");
    if (!s.ppv.previous) return s.ppv.previous = sessionStorage.getItem(s.ppv.sessionStorageKey) || "", s.ppv.init(), s.ppv.previous.split(",")
};
s.ppv = {
    initialPercent: 0,
    maxPercent: 0,
    throttleAmount: 500,
    sessionStorageKey: "s_ppv",
    init: function() {
        window.addEventListener("scroll", s.ppv.throttle(s.ppv.scroll, s.ppv.throttleAmount), !1);
        window.addEventListener("resize", s.ppv.throttle(s.ppv.scroll, s.ppv.throttleAmount), !1);
        window.addEventListener("beforeunload", s.ppv.unload, !1);
        window.addEventListener("load", s.ppv.scroll, !1)
    },
    scroll: function() {
        var a = s.ppv;
        if (100 != a.maxPercent) {
            var g = void 0 !== window.pageYOffset ? window.pageYOffset : (document.documentElement || document.body.parentNode || document.body).scrollTop,
                d = document.clientHeight || document.documentElement.clientHeight || document.body.clientHeight,
                b = a.getDocHeight(),
                b = Math.round((g + d) / b * 100);
            a.initialPercent || (a.initialPercent = b);
            if (b > a.maxPercent) {
                a.maxPercent = b;
                var c = [];
                c.push(s.pageName);
                c.push(b);
                c.push(a.initialPercent);
                c.push(g + d);
                sessionStorage.setItem(a.sessionStorageKey, c.join(","))
            }
        }
    },
    getDocHeight: function() {
        var a = window.document;
        return Math.max(Math.max(a.body.scrollHeight, a.documentElement.scrollHeight), Math.max(a.body.offsetHeight, a.documentElement.offsetHeight), Math.max(a.body.clientHeight, a.documentElement.clientHeight))
    },
    unload: function() {
        sessionStorage.getItem(s.ppv.sessionStorageKey) && sessionStorage.setItem(s.ppv.sessionStorageKey, sessionStorage.getItem(s.ppv.sessionStorageKey))
    },
    throttle: function(a, g) {
        var d, b, c, e = null,
            f = 0,
            l = function() {
                f = new Date;
                e = null;
                c = a.apply(d, b)
            };
        return function() {
            var h = new Date;
            f || (f = h);
            var k = g - (h - f);
            d = this;
            b = arguments;
            0 >= k ? (clearTimeout(e), e = null, f = h, c = a.apply(d, b)) : e || (e = setTimeout(l, k));
            return c
        }
    }
};

s.pt=function(x,d,f,a){var s=this,t=x,z=0,y,r;while(t){y=t.indexOf(d);y=y<0?t.length:y;t=t.substring(0,y);r=s[f](t,a);if(r)return r;z+=y+d.length;t=x.substring(z,x.length);t=z<x.length?t:''}return ''};
s.getQueryParam=function getQueryParam(a,d,f){function n(g,c){c=c.split("?").join("&");c=c.split("#").join("&");var e=c.indexOf("&");if(g&&(-1<e||c.indexOf("=")>e)){e=c.substring(e+1);e=e.split("&");for(var h=0,p=e.length;h<p;h++){var l=e[h].split("="),q=l[1];if(l[0].toLowerCase()===g.toLowerCase())return decodeURIComponent(q||!0)}}return""}if("-v"===a)return{plugin:"getQueryParam",version:"4.0.1"};var b=function(){if("undefined"!==typeof window.s_c_il)for(var g=0,c;g<window.s_c_il.length;g++)if(c=window.s_c_il[g],c._c&&"s_c"===c._c)return c}();"undefined"!==typeof b&&(b.contextData.getQueryParam="4.0");if(a){d=d||"";f=(f||"undefined"!==typeof b&&b.pageURL||location.href)+"";(4<d.length||-1<d.indexOf("="))&&f&&4>f.length&&(b=d,d=f,f=b);b="";for(var m=a.split(","),r=m.length,k=0;k<r;k++)a=n(m[k],f),"string"===typeof a?(a=-1<a.indexOf("#")?a.substring(0,a.indexOf("#")):a,b+=b?d+a:a):b=""===b?a:b+(d+a);return b}};
s.getValOnce=function getValOnce(vtc,cn,et,ep){var e=vtc,i=cn,t=et,n=ep;  if(arguments&&"-v"===arguments[0])return{plugin:"getValOnce",version:"3.1"};var o=function(){if(void 0!==window.s_c_il){for(var e,i=0;i<window.s_c_il.length;i++)if((e=window.s_c_il[i])._c&&"s_c"===e._c)return e}}();if(void 0!==o&&(o.contextData.getValOnce="3.1"),window.cookieWrite=window.cookieWrite||function(e,i,t){if("string"==typeof e){var n=window.location.hostname,o=window.location.hostname.split(".").length-1;if(n&&!/^[0-9.]+$/.test(n)){o=2<o?o:2;var r=n.lastIndexOf(".");if(0<=r){for(;0<=r&&1<o;)r=n.lastIndexOf(".",r-1),o--;r=0<r?n.substring(r):n}}if(g=r,i=void 0!==i?""+i:"",t||""===i){if(""===i&&(t=-60),"number"==typeof t){var f=new Date;f.setTime(f.getTime()+6e4*t)}else f=t}return!!e&&(document.cookie=encodeURIComponent(e)+"="+encodeURIComponent(i)+"; path=/;"+(t?" expires="+f.toUTCString()+";":"")+(g?" domain="+g+";":""),"undefined"!=typeof cookieRead)&&cookieRead(e)===i}},window.cookieRead=window.cookieRead||function(e){if("string"!=typeof e)return"";e=encodeURIComponent(e);var i=" "+document.cookie,t=i.indexOf(" "+e+"="),n=0>t?t:i.indexOf(";",t);return(e=0>t?"":decodeURIComponent(i.substring(t+2+e.length,0>n?i.length:n)))?e:""},e){var i=i||"s_gvo",t=t||0,n="m"===n?6e4:864e5;if(e!==cookieRead(i)){var r=new Date;return r.setTime(r.getTime()+t*n),cookieWrite(i,e,0===t?0:r),e}}return""}

// s_sp = new Function("x", "d", "var a=new Array,i=0,j;if(x){if(x.split)a=x.split(d);else if(!d)for(i=0;i<x.length;i++)a[a.length]=x.substring(i,i+1);else while(i>=0){j=x.indexOf(d,i);a[a.length]=x.subst" + "ring(i,j<0?x.length:j);i=j;if(i>=0)i+=d.length}}return a");
// s_jn = new Function("a", "d", "var x='',i,j=a.length;if(a&&j>0){x=a[0];if(j>1){if(a.join)x=a.join(d);else for(i=1;i<j;i++)x+=d+a[i]}}return x");
// s.rep = new Function("x", "o", "n", "return s_jn(s_sp(x,o),n)");
// s.epa=function(x){var s=this,y,tcf;if(x){x=s.rep(''+x,'+',' ');if(s.em==3){tcf=new Function('x','var y,e;try{y=decodeURIComponent(x)}catch(e){y=unescap" + "e(x)}return y');return tcf(x)}else return unescape(x)}return y};
//s.pt=function(l,de,cf,fa){if(l&&this[cf]){l=l.split(de||",");de=l.length;for(var e,c=0;c<de;c++)if(e=this[cf](l[c],fa))return e}};
// s.detectRIA = new Function("cn", "fp", "sp", "mfv", "msv", "sf", "" + "cn=cn?cn:'s_ria';msv=msv?msv:2;mfv=mfv?mfv:10;var s=this,sv='',fv=-" + "1,dwi=0,fr='',sr='',w,mt=s.n.mimeTypes,uk=s.c_r(cn),k=s.c_w('s_cc'," + "'true',0)?'Y':'N';fk=uk.substring(0,uk.indexOf('|'));sk=uk.substrin" + "g(uk.indexOf('|')+1,uk.length);if(k=='Y'&&s.p_fo('detectRIA')){if(u" + "k&&!sf){if(fp){s[fp]=fk;}if(sp){s[sp]=sk;}return false;}if(!fk&&fp)" + "{if(s.pl&&s.pl.length){if(s.pl['Shockwave Flash 2.0'])fv=2;x=s.pl['" + "Shockwave Flash'];if(x){fv=0;z=x.description;if(z)fv=z.substring(16" + ",z.indexOf('.'));}}else if(navigator.plugins&&navigator.plugins.len" + "gth){x=navigator.plugins['Shockwave Flash'];if(x){fv=0;z=x.descript" + "ion;if(z)fv=z.substring(16,z.indexOf('.'));}}else if(mt&&mt.length)" + "{x=mt['application/x-shockwave-flash'];if(x&&x.enabledPlugin)fv=0;}" + "if(fv<=0)dwi=1;w=s.u.indexOf('Win')!=-1?1:0;if(dwi&&s.isie&&w&&exec" + "Script){result=false;for(var i=mfv;i>=3&&result!=true;i--){execScri" + "pt('on error resume next: result = IsObject(CreateObject(\"Shockwav" + "eFlash.ShockwaveFlash.'+i+'\"))','VBScript');fv=i;}}fr=fv==-1?'Flas" + "h Not Detected':fv==0?'Flash Enabled (No Version)':'Flash '+fv;}if(" + "!sk&&sp&&s.apv>=4.1){var tc='try{x=new ActiveXObject(\"AgControl.A'" + "+'gControl\");for(var i=msv;i>0;i--){for(var j=9;j>=0;j--){if(x.is'" + '+\'VersionSupported(i+"."+j)){sv=i+"."+j;break;}}if(sv){break;}\'' + "+'}}catch(e){try{x=navigator.plugins[\"Silverlight Plug-In\"];sv=x'" + "+'.description.substring(0,x.description.indexOf(\".\")+2);}catch('" + "+'e){}}';eval(tc);sr=sv==''?'Silverlight Not Detected':'Silverlight" + " '+sv;}if((fr&&fp)||(sr&&sp)){s.c_w(cn,fr+'|'+sr,0);if(fr)s[fp]=fr;" + "if(sr)s[sp]=sr;}}");
// s.downloadLinkHandler = new Function("p", "" + "var s=this,h=s.p_gh(),n='linkDownloadFileTypes',i,t;if(!h||(s.linkT" + "ype&&(h||s.linkName)))return '';i=h.indexOf('?');t=s[n];s[n]=p?p:t;" + "if(s.lt(h)=='d')s.linkType='d';else h='';s[n]=t;return h;");
// s.linkHandler = new Function("p", "t", "" + "var s=this,h=s.p_gh(),i,l;t=t?t:'o';if(!h||(s.linkType&&(h||s.linkN" + "ame)))return '';i=h.indexOf('?');h=s.linkLeaveQueryString||i<0?h:h." + "substring(0,i);l=s.pt(p,'|','p_gn',h.toLowerCase());if(l){s.linkNam" + "e=l=='[['?'':l;s.linkType=t;return h;}return '';");
// s.p_gn = new Function("t", "h", "" + "var i=t?t.indexOf('~'):-1,n,x;if(t&&h){n=i<0?'':t.substring(0,i);x=" + "t.substring(i+1);if(h.indexOf(x.toLowerCase())>-1)return n?n:'[[';}" + "return 0;");
// s.getPreviousValue = new Function("v", "c", "el", "" + "var s=this,t=new Date,i,j,r='';t.setTime(t.getTime()+1800000);if(el" + "){if(s.events){i=s.split(el,',');j=s.split(s.events,',');for(x in i" + "){for(y in j){if(i[x]==j[y]){if(s.c_r(c)) r=s.c_r(c);v?s.c_w(c,v,t)" + ":s.c_w(c,'no value',t);return r}}}}}else{if(s.c_r(c)) r=s.c_r(c);v?" + "s.c_w(c,v,t):s.c_w(c,'no value',t);return r}");
//s.setupDynamicObjectIDs = new Function("" + "var s=this;if(!s.doi){s.doi=1;if(s.apv>3&&(!s.isie||!s.ismac||s.apv" + ">=5)){if(s.wd.attachEvent)s.wd.attachEvent('onload',s.setOIDs);else" + " if(s.wd.addEventListener)s.wd.addEventListener('load',s.setOIDs,fa" + "lse);else{s.doiol=s.wd.onload;s.wd.onload=s.setOIDs}}s.wd.s_semapho" + "re=1}");
// s.setOIDs = new Function("e", "" + "var s=s_c_il[" + s._in + "],b=s.eh(s.wd,'onload'),o='onclick',x,l,u,c,i" + ",a=new Array;if(s.doiol){if(b)s[b]=s.wd[b];s.doiol(e)}if(s.d.links)" + "{for(i=0;i<s.d.links.length;i++){l=s.d.links[i];" + "if(s._isSafari){safariHandler(l);}" + "c=l[o]?''+l[o]:'';b" + "=s.eh(l,o);z=l[b]?''+l[b]:'';u=s.getObjectID(l);if(u&&c.indexOf('s_" + "objectID')<0&&z.indexOf('s_objectID')<0){u=s.repl(u,'\"','');u=s.re" + "pl(u,'\\n','').substring(0,97);l.s_oc=l[o];a[u]=a[u]?a[u]+1:1;x='';" + "if(c.indexOf('.t(')>=0||c.indexOf('.tl(')>=0||c.indexOf('s_gs(')>=0" + ")x='var x=\".tl(\";';x+='s_objectID=\"'+u+'_'+a[u]+'\";return this." + "s_oc?this.s_oc(e):true';if(s.isns&&s.apv>=5)l.setAttribute(o,x);l[o" + "]=new Function('e',x)}}}s.wd.s_semaphore=0;return true");
// s.getQueryParam = new Function("p", "d", "u", "" + "var s=this,v='',i,t;d=d?d:'';u=u?u:(s.pageURL?s.pageURL:s.wd.locati" + "on);if(u=='f')u=s.gtfs().location;while(p){i=p.indexOf(',');i=i<0?p" + ".length:i;t=s.p_gpv(p.substring(0,i),u+'');if(t){t=t.indexOf('#')>-" + "1?t.substring(0,t.indexOf('#')):t;}if(t)v+=v?d+t:t;p=p.substring(i=" + "=p.length?i:i+1)}return v");
// s.p_gpv = new Function("k", "u", "" + "var s=this,v='',i=u.indexOf('?'),q;if(k&&i>-1){q=u.substring(i+1);v" + "=s.pt(q,'&','p_gvf',k)}return v");
// s.p_gvf = new Function("t", "k", "" + "if(t){var s=this,i=t.indexOf('='),p=i<0?t:t.substring(0,i),v=i<0?'T" + "rue':t.substring(i+1);if(p.toLowerCase()==k.toLowerCase())return s." + "epa(v)}return ''");
// s.getValOnce = new Function("v", "c", "e", "" + "var s=this,a=new Date,v=v?v:v='',c=c?c:c='s_gvo',e=e?e:0,k=s.c_r(c" + ");if(v){a.setTime(a.getTime()+e*86400000);s.c_w(c,v,e?a:0);}return" + " v==k?'':v");
// s.setClickMapEmail = new Function("qp", "ot", "" + "var s=this,v=s.getQueryParam(qp,'~'),d,pn,oid,ot=s.getQueryParam(ot)" + ",ot=ot?ot:'A',cv;d=v.indexOf('~');if(!v)return '';if(d>-1){pn=v.subs" + "tring(0,d);oid=v.substring(d+1);}cv='&pid='+s.ape(s.fl(pn,255))+'&pi" + "dt=1&oid='+s.ape(s.fl(oid,100))+'&oidt=1&ot='+ot+'&oi=1';s.sq(cv);");
// s.getAndPersistValue = new Function("v", "c", "e", "" + "var s=this,a=new Date;e=e?e:0;a.setTime(a.getTime()+e*86400000);if(" + "v)s.c_w(c,v,e?a:0);return s.c_r(c);");
// s.__se = new Function("" + "var l={'~':'tl:[\\'','^': 'kw:[\\'','%': 'ahoo','|': '\\'],','>': '" + "\\']}','*': '.com','$': 'search',';':'query','#':'land','`':'oogle'" + ",'+':'http://www','<':'keyword'};var f=this.___se+'';var g='';for(v" + "ar i=0;i<f.length;i++){if(l[f.substring(i,i+1)]&&typeof l[f.substri" + "ng(i,i+1)]!='undefined'){g+=l[f.substring(i,i+1)];}else{g+=f.substr" + "ing(i,i+1);}}return eval('('+g+')');");
// s.___se = "{}";
// s.isEntry = new Function("" + "var s=this;var l=s.linkInternalFilters,r=s.referrer||typeof s.refer" + "rer!='undefined'?s.referrer:document.referrer,p=l.indexOf(','),b=0," + "v='';if(!r){return 1;}while(p=l.indexOf(',')){v=p>-1?l.substring(b," + "p):l;if(v=='.'||r.indexOf(v)>-1){return 0;}if(p==-1){break;}b=p+1;l" + "=l.substring(b,l.length);}return 1;");
// s.p_fo = new Function("n", "" + "var s=this;if(!s.__fo){s.__fo=new Object;}if(!s.__fo[n]){s.__fo[n]=" + "new Object;return 1;}else {return 0;}");
// s.manageVars = new Function("c", "l", "f", "" + "var s=this,vl,la,vla;l=l?l:'';f=f?f:1 ;if(!s[c])return false;vl='pa" + "geName,purchaseID,channel,server,pageType,campaign,state,zip,events" + ",products,transactionID';for(var n=1;n<76;n++){vl+=',prop'+n+',eVar" + "'+n+',hier'+n;}if(l&&(f==1||f==2)){if(f==1){vl=l;}if(f==2){la=s.spl" + "it(l,',');vla=s.split(vl,',');vl='';for(x in la){for(y in vla){if(l" + "a[x]==vla[y]){vla[y]='';}}}for(y in vla){vl+=vla[y]?','+vla[y]:'';}" + "}s.pt(vl,',',c,0);return true;}else if(l==''&&f==1){s.pt(vl,',',c,0" + ");return true;}else{return false;}");
// s.clearVars = new Function("t", "var s=this;s[t]='';");
// s.lowercaseVars = new Function("t", "" + "var s=this;if(s[t]&&t!='events'){s[t]=s[t].toString();if(s[t].index" + "Of('D=')!=0){s[t]=s[t].toLowerCase();}}");
// s.join = new Function("v", "p", "" + "var s = this;var f,b,d,w;if(p){f=p.front?p.front:'';b=p.back?p.back" + ":'';d=p.delim?p.delim:'';w=p.wrap?p.wrap:'';}var str='';for(var x=0" + ";x<v.length;x++){if(typeof(v[x])=='object' )str+=s.join( v[x],p);el" + "se str+=w+v[x]+w;if(x<v.length-1)str+=d;}return f+str+b;");
// s.p_gh = new Function("" + "var s=this;if(!s.eo&&!s.lnk)return '';var o=s.eo?s.eo:s.lnk,y=s.ot(" + "o),n=s.oid(o),x=o.s_oidt;if(s.eo&&o==s.eo){while(o&&!n&&y!='BODY'){" + "o=o.parentElement?o.parentElement:o.parentNode;if(!o)return '';y=s." + "ot(o);n=s.oid(o);x=o.s_oidt}}return o.href?o.href:'';");
// s.apl = new Function("L", "v", "d", "u", "" + "var s=this,m=0;if(!L)L='';if(u){var i,n,a=s.split(L,d);for(i=0;i<a." + "length;i++){n=a[i];m=m||(u==1?(n==v):(n.toLowerCase()==v.toLowerCas" + "e()));}}if(!m)L=L?L+d+v:v;return L");
// s.repl = new Function("x", "o", "n", "" + "var i=x.indexOf(o),l=n.length;while(x&&i>=0){x=x.substring(0,i)+n+x." + "substring(i+o.length);i=x.indexOf(o,i+l)}return x");
// s.split = new Function("l", "d", "" + "var i,x=0,a=new Array;while(l){i=l.indexOf(d);i=i>-1?i:l.length;a[x" + "++]=l.substring(0,i);l=l.substring(i+d.length);}return a");
// s.vpr = new Function("vs", "v", "if(typeof(v)!='undefined'){var s=this; eval('s.'+vs+'=\"'+v+'\"')}");
s.trackingServer = "metrics.apple.com";
s.trackingServerSecure = "securemetrics.apple.com";
s.dc = 112;

// function AppMeasurement_Module_Media(q){var b=this;b.s=q;q=window;q.s_c_in||(q.s_c_il=[],q.s_c_in=0);b._il=q.s_c_il;b._in=q.s_c_in;b._il[b._in]=b;q.s_c_in++;b._c="s_m";b.list=[];b.open=function(d,c,e,k){var f={},a=new Date,l="",g;c||(c=-1);if(d&&e){b.list||(b.list={});b.list[d]&&b.close(d);k&&k.id&&(l=k.id);if(l)for(g in b.list)!Object.prototype[g]&&b.list[g]&&b.list[g].R==l&&b.close(b.list[g].name);f.name=d;f.length=c;f.offset=0;f.e=0;f.playerName=b.playerName?b.playerName:e;f.R=l;f.C=0;f.a=0;f.timestamp=
// Math.floor(a.getTime()/1E3);f.k=0;f.u=f.timestamp;f.c=-1;f.n="";f.g=-1;f.D=0;f.I={};f.G=0;f.m=0;f.f="";f.B=0;f.L=0;f.A=0;f.F=0;f.l=!1;f.v="";f.J="";f.K=0;f.r=!1;f.H="";f.complete=0;f.Q=0;f.p=0;f.q=0;b.list[d]=f}};b.openAd=function(d,c,e,k,f,a,l,g){var h={};b.open(d,c,e,g);if(h=b.list[d])h.l=!0,h.v=k,h.J=f,h.K=a,h.H=l};b.M=function(d){var c=b.list[d];b.list[d]=0;c&&c.monitor&&clearTimeout(c.monitor.interval)};b.close=function(d){b.i(d,0,-1)};b.play=function(d,c,e,k){var f=b.i(d,1,c,e,k);f&&!f.monitor&&
// (f.monitor={},f.monitor.update=function(){1==f.k&&b.i(f.name,3,-1);f.monitor.interval=setTimeout(f.monitor.update,1E3)},f.monitor.update())};b.click=function(d,c){b.i(d,7,c)};b.complete=function(d,c){b.i(d,5,c)};b.stop=function(d,c){b.i(d,2,c)};b.track=function(d){b.i(d,4,-1)};b.P=function(d,c){var e="a.media.",k=d.linkTrackVars,f=d.linkTrackEvents,a="m_i",l,g=d.contextData,h;c.l&&(e+="ad.",c.v&&(g["a.media.name"]=c.v,g[e+"pod"]=c.J,g[e+"podPosition"]=c.K),c.G||(g[e+"CPM"]=c.H));c.r&&(g[e+"clicked"]=
// !0,c.r=!1);g["a.contentType"]="video"+(c.l?"Ad":"");g["a.media.channel"]=b.channel;g[e+"name"]=c.name;g[e+"playerName"]=c.playerName;0<c.length&&(g[e+"length"]=c.length);g[e+"timePlayed"]=Math.floor(c.a);0<Math.floor(c.a)&&(g[e+"timePlayed"]=Math.floor(c.a));c.G||(g[e+"view"]=!0,a="m_s",b.Heartbeat&&b.Heartbeat.enabled&&(a=c.l?b.__primetime?"mspa_s":"msa_s":b.__primetime?"msp_s":"ms_s"),c.G=1);c.f&&(g[e+"segmentNum"]=c.m,g[e+"segment"]=c.f,0<c.B&&(g[e+"segmentLength"]=c.B),c.A&&0<c.a&&(g[e+"segmentView"]=
// !0));!c.Q&&c.complete&&(g[e+"complete"]=!0,c.S=1);0<c.p&&(g[e+"milestone"]=c.p);0<c.q&&(g[e+"offsetMilestone"]=c.q);if(k)for(h in g)Object.prototype[h]||(k+=",contextData."+h);l=g["a.contentType"];d.pe=a;d.pev3=l;var q,s;if(b.contextDataMapping)for(h in d.events2||(d.events2=""),k&&(k+=",events"),b.contextDataMapping)if(!Object.prototype[h]){a=h.length>e.length&&h.substring(0,e.length)==e?h.substring(e.length):"";l=b.contextDataMapping[h];if("string"==typeof l)for(q=l.split(","),s=0;s<q.length;s++)l=
// q[s],"a.contentType"==h?(k&&(k+=","+l),d[l]=g[h]):"view"==a||"segmentView"==a||"clicked"==a||"complete"==a||"timePlayed"==a||"CPM"==a?(f&&(f+=","+l),"timePlayed"==a||"CPM"==a?g[h]&&(d.events2+=(d.events2?",":"")+l+"="+g[h]):g[h]&&(d.events2+=(d.events2?",":"")+l)):"segment"==a&&g[h+"Num"]?(k&&(k+=","+l),d[l]=g[h+"Num"]+":"+g[h]):(k&&(k+=","+l),d[l]=g[h]);else if("milestones"==a||"offsetMilestones"==a)h=h.substring(0,h.length-1),g[h]&&b.contextDataMapping[h+"s"][g[h]]&&(f&&(f+=","+b.contextDataMapping[h+
// "s"][g[h]]),d.events2+=(d.events2?",":"")+b.contextDataMapping[h+"s"][g[h]]);g[h]&&(g[h]=0);"segment"==a&&g[h+"Num"]&&(g[h+"Num"]=0)}d.linkTrackVars=k;d.linkTrackEvents=f};b.i=function(d,c,e,k,f){var a={},l=(new Date).getTime()/1E3,g,h,q=b.trackVars,s=b.trackEvents,t=b.trackSeconds,u=b.trackMilestones,v=b.trackOffsetMilestones,w=b.segmentByMilestones,x=b.segmentByOffsetMilestones,p,n,r=1,m={},y;b.channel||(b.channel=b.s.w.location.hostname);if(a=d&&b.list&&b.list[d]?b.list[d]:0)if(a.l&&(t=b.adTrackSeconds,
// u=b.adTrackMilestones,v=b.adTrackOffsetMilestones,w=b.adSegmentByMilestones,x=b.adSegmentByOffsetMilestones),0>e&&(e=1==a.k&&0<a.u?l-a.u+a.c:a.c),0<a.length&&(e=e<a.length?e:a.length),0>e&&(e=0),a.offset=e,0<a.length&&(a.e=a.offset/a.length*100,a.e=100<a.e?100:a.e),0>a.c&&(a.c=e),y=a.D,m.name=d,m.ad=a.l,m.length=a.length,m.openTime=new Date,m.openTime.setTime(1E3*a.timestamp),m.offset=a.offset,m.percent=a.e,m.playerName=a.playerName,m.mediaEvent=0>a.g?"OPEN":1==c?"PLAY":2==c?"STOP":3==c?"MONITOR":
// 4==c?"TRACK":5==c?"COMPLETE":7==c?"CLICK":"CLOSE",2<c||c!=a.k&&(2!=c||1==a.k)){f||(k=a.m,f=a.f);if(c){1==c&&(a.c=e);if((3>=c||5<=c)&&0<=a.g&&(r=!1,q=s="None",a.g!=e)){h=a.g;h>e&&(h=a.c,h>e&&(h=e));p=u?u.split(","):0;if(0<a.length&&p&&e>=h)for(n=0;n<p.length;n++)(g=p[n]?parseFloat(""+p[n]):0)&&h/a.length*100<g&&a.e>=g&&(r=!0,n=p.length,m.mediaEvent="MILESTONE",a.p=m.milestone=g);if((p=v?v.split(","):0)&&e>=h)for(n=0;n<p.length;n++)(g=p[n]?parseFloat(""+p[n]):0)&&h<g&&e>=g&&(r=!0,n=p.length,m.mediaEvent=
// "OFFSET_MILESTONE",a.q=m.offsetMilestone=g)}if(a.L||!f){if(w&&u&&0<a.length){if(p=u.split(","))for(p.push("100"),n=h=0;n<p.length;n++)if(g=p[n]?parseFloat(""+p[n]):0)a.e<g&&(k=n+1,f="M:"+h+"-"+g,n=p.length),h=g}else if(x&&v&&(p=v.split(",")))for(p.push(""+(0<a.length?a.length:"E")),n=h=0;n<p.length;n++)if((g=p[n]?parseFloat(""+p[n]):0)||"E"==p[n]){if(e<g||"E"==p[n])k=n+1,f="O:"+h+"-"+g,n=p.length;h=g}f&&(a.L=!0)}(f||a.f)&&f!=a.f&&(a.F=!0,a.f||(a.m=k,a.f=f),0<=a.g&&(r=!0));(2<=c||100<=a.e)&&a.c<e&&
// (a.C+=e-a.c,a.a+=e-a.c);if(2>=c||3==c&&!a.k)a.n+=(1==c||3==c?"S":"E")+Math.floor(e),a.k=3==c?1:c;!r&&0<=a.g&&3>=c&&(t=t?t:0)&&a.a>=t&&(r=!0,m.mediaEvent="SECONDS");a.u=l;a.c=e}if(!c||3>=c&&100<=a.e)2!=a.k&&(a.n+="E"+Math.floor(e)),c=0,q=s="None",m.mediaEvent="CLOSE";7==c&&(r=m.clicked=a.r=!0);if(5==c||b.completeByCloseOffset&&(!c||100<=a.e)&&0<a.length&&e>=a.length-b.completeCloseOffsetThreshold)r=m.complete=a.complete=!0;l=m.mediaEvent;"MILESTONE"==l?l+="_"+m.milestone:"OFFSET_MILESTONE"==l&&(l+=
// "_"+m.offsetMilestone);a.I[l]?m.eventFirstTime=!1:(m.eventFirstTime=!0,a.I[l]=1);m.event=m.mediaEvent;m.timePlayed=a.C;m.segmentNum=a.m;m.segment=a.f;m.segmentLength=a.B;b.monitor&&4!=c&&b.monitor(b.s,m);b.Heartbeat&&b.Heartbeat.enabled&&0<=a.g&&(r=!1);0==c&&b.M(d);r&&a.D==y&&(d={contextData:{}},d.linkTrackVars=q,d.linkTrackEvents=s,d.linkTrackVars||(d.linkTrackVars=""),d.linkTrackEvents||(d.linkTrackEvents=""),b.P(d,a),d.linkTrackVars||(d["!linkTrackVars"]=1),d.linkTrackEvents||(d["!linkTrackEvents"]=
// 1),b.s.track(d),a.F?(a.m=k,a.f=f,a.A=!0,a.F=!1):0<a.a&&(a.A=!1),a.n="",a.p=a.q=0,a.a-=Math.floor(a.a),a.g=e,a.D++)}return a};b.O=function(d,c,e,k,f){var a=0;if(d&&(!b.autoTrackMediaLengthRequired||c&&0<c)){if(b.list&&b.list[d])a=1;else if(1==e||3==e)b.open(d,c,"HTML5 Video",f),a=1;a&&b.i(d,e,k,-1,0)}};b.attach=function(d){var c,e,k;d&&d.tagName&&"VIDEO"==d.tagName.toUpperCase()&&(b.o||(b.o=function(c,a,d){var e,h;b.autoTrack&&(e=c.currentSrc,(h=c.duration)||(h=-1),0>d&&(d=c.currentTime),b.O(e,h,a,
// d,c))}),c=function(){b.o(d,1,-1)},e=function(){b.o(d,1,-1)},b.j(d,"play",c),b.j(d,"pause",e),b.j(d,"seeking",e),b.j(d,"seeked",c),b.j(d,"ended",function(){b.o(d,0,-1)}),b.j(d,"timeupdate",c),k=function(){d.paused||d.ended||d.seeking||b.o(d,3,-1);setTimeout(k,1E3)},k())};b.j=function(b,c,e){b.attachEvent?b.attachEvent("on"+c,e):b.addEventListener&&b.addEventListener(c,e,!1)};void 0==b.completeByCloseOffset&&(b.completeByCloseOffset=1);void 0==b.completeCloseOffsetThreshold&&(b.completeCloseOffsetThreshold=
// 1);b.Heartbeat={};b.N=function(){var d,c;if(b.autoTrack&&(d=b.s.d.getElementsByTagName("VIDEO")))for(c=0;c<d.length;c++)b.attach(d[c])};b.j(q,"load",b.N)}


/*
 ============== DO NOT ALTER ANYTHING BELOW THIS LINE ! ===============
 AppMeasurement for JavaScript version: 2.27.0
 Implementation guide: https://adobe.ly/40OOIRT
*/
function AppMeasurement(s){var a=this;a.version="2.27.0";var k=window;k.s_c_in||(k.s_c_il=[],k.s_c_in=0);a._il=k.s_c_il;a._in=k.s_c_in;a._il[a._in]=a;k.s_c_in++;a._c="s_c";var m,q;try{m=k.AppMeasurement,(q=m.Cc)||(q=null)}catch(v){}var p=k,r,t;try{for(r=p.parent,t=p.location;r&&r.location&&t&&""+r.location!==""+t&&p.location&&""+r.location!==""+p.location&&r.location.host===t.host;)p=r,r=p.parent}catch(w){}a.log=function(a){try{console.log(a)}catch(c){}};a.bb=function(a){return""+parseInt(a)==""+
a};a.replace=function(a,c,d){return!a||0>a.indexOf(c)?a:a.split(c).join(d)};a.escape=function(b){var c,d;if(!b)return b;b=encodeURIComponent(b);for(c=0;7>c;c++)d="+~!*()'".substring(c,c+1),0<=b.indexOf(d)&&(b=a.replace(b,d,"%"+d.charCodeAt(0).toString(16).toUpperCase()));return b};a.unescape=function(b){if(!b)return b;b=0<=b.indexOf("+")?a.replace(b,"+"," "):b;try{return decodeURIComponent(b)}catch(c){}return unescape(b)};a.zb=function(b){var c=a.fpCookieDomainPeriods,d;if(!a.Va&&b&&!a.Kb(b)&&(c||
(c=a.cookieDomainPeriods),c||(c=a.yb(b)),c=c?parseInt(c):2,c=2<c?c:2,d=b.lastIndexOf("."),0<=d)){for(;0<=d&&1<c;)d=b.lastIndexOf(".",d-1),c--;a.Va=0<d?b.substring(d):b}return a.Va};a.Kb=function(a){return/^(?:\d{1,3}\.){3}\d{1,3}$/.test(a)||/^([a-f0-9:]+:+)+[a-f0-9]+$/.test(a)};a.vb=function(a){var c=[];a=a?a.split("."):[];var d;for(d=a.length-1;0<=d;d--)c.push(a.slice(d).join("."));return c};a.yb=function(b){var c="";b=a.vb(b);for(var d=0;d<b.length&&!a.cookieRead("s_ac");)c=b[d],d+=1,a.d.cookie=
a.W("s_ac","1",{path:"/",domain:c,ya:a.writeSecureCookies});a.d.cookie=a.W("s_ac","",{path:"/",domain:c,ya:a.writeSecureCookies,ma:new Date(0)});return d};a.c_r=a.cookieRead=function(b){b=a.escape(b);var c=" "+a.d.cookie,d=c.indexOf(" "+b+"="),f=0>d?d:c.indexOf(";",d);b=0>d?"":a.unescape(c.substring(d+2+b.length,0>f?c.length:f));return"[[B]]"!=b?b:""};a.c_w=a.cookieWrite=function(b,c,d){var f=a.zb(k.location.hostname),e=a.cookieLifetime,g;c=""+c;e=e?(""+e).toUpperCase():"";d&&"SESSION"!==e&&"NONE"!==
e&&((g=""!=c?parseInt(e?e:0):-60)?(d=new Date,d.setTime(d.getTime()+1E3*g)):1===d&&(d=new Date,g=d.getYear(),d.setYear(g+2+(1900>g?1900:0))));return b&&"NONE"!==e?(f={path:"/",domain:f,ya:a.writeSecureCookies},"SESSION"!==e&&(f.ma=d),a.d.cookie=a.W(b,c,f),a.cookieRead(b)==c):!1};a.W=function(b,c,d){d=d||{};if(!b)return"";c=""!==c?c:"[[B]]";b=a.escape(b)+"="+a.escape(c);d.path&&(b+="; path="+d.path);d.ma&&(b+="; expires="+(new Date(d.ma)).toUTCString());d.domain&&(b+="; domain="+d.domain);d.ya&&(b+=
"; secure");return b};a.gc=function(){var b=a.Util.getIeVersion();"number"===typeof b&&10>b&&(a.unsupportedBrowser=!0,a.Rb(a,function(){}))};a.Ia=function(){var a=navigator.userAgent;return"Microsoft Internet Explorer"===navigator.appName||0<=a.indexOf("MSIE ")||0<=a.indexOf("Trident/")&&0<=a.indexOf("Windows NT 6")?!0:!1};a.Rb=function(a,c){for(var d in a)Object.prototype.hasOwnProperty.call(a,d)&&"function"===typeof a[d]&&(a[d]=c)};a.M=[];a.ka=function(b,c,d){if(a.Wa)return 0;a.maxDelay||(a.maxDelay=
250);var f=0,e=(new Date).getTime()+a.maxDelay,g=a.d.visibilityState,h=["webkitvisibilitychange","visibilitychange"];g||(g=a.d.webkitVisibilityState);if(g&&"prerender"==g){if(!a.la)for(a.la=1,d=0;d<h.length;d++)a.d.addEventListener(h[d],function(){var b=a.d.visibilityState;b||(b=a.d.webkitVisibilityState);"visible"==b&&(a.la=0,a.delayReady())});f=1;e=0}else d||a.v("_d")&&(f=1);f&&(a.M.push({m:b,a:c,t:e}),a.la||setTimeout(a.delayReady,a.maxDelay));return f};a.delayReady=function(){var b=(new Date).getTime(),
c=0,d;for(a.v("_d")?c=1:a.Ka();0<a.M.length;){d=a.M.shift();if(c&&!d.t&&d.t>b){a.M.unshift(d);setTimeout(a.delayReady,parseInt(a.maxDelay/2));break}a.Wa=1;a[d.m].apply(a,d.a);a.Wa=0}};a.setAccount=a.sa=function(b){var c,d;if(!a.ka("setAccount",arguments))if(a.account=b,a.allAccounts)for(c=a.allAccounts.concat(b.split(",")),a.allAccounts=[],c.sort(),d=0;d<c.length;d++)0!=d&&c[d-1]==c[d]||a.allAccounts.push(c[d]);else a.allAccounts=b.split(",")};a.foreachVar=function(b,c){var d,f,e,g,h="";e=f="";if(a.lightProfileID)d=
a.Q,(h=a.lightTrackVars)&&(h=","+h+","+a.ra.join(",")+",");else{d=a.i;if(a.pe||a.linkType)h=a.linkTrackVars,f=a.linkTrackEvents,a.pe&&(e=a.pe.substring(0,1).toUpperCase()+a.pe.substring(1),a[e]&&(h=a[e].yc,f=a[e].xc));h&&(h=","+h+","+a.F.join(",")+",");f&&h&&(h+=",events,")}c&&(c=","+c+",");for(f=0;f<d.length;f++)e=d[f],(g=a[e])&&(!h||0<=h.indexOf(","+e+","))&&(!c||0<=c.indexOf(","+e+","))&&b(e,g)};a.l=function(b,c,d,f,e){var g="",h,l,k,n,m=0;"contextData"==b&&(b="c");"clientHints"==b&&(b="h");if(c){for(h in c)if(!(Object.prototype[h]||
e&&h.substring(0,e.length)!=e)&&c[h]&&(!d||0<=d.indexOf(","+(f?f+".":"")+h+","))){k=!1;if(m)for(l=0;l<m.length;l++)if(h.substring(0,m[l].length)==m[l]){k=!0;break}if(!k&&(""==g&&(g+="&"+b+"."),l=c[h],e&&(h=h.substring(e.length)),0<h.length))if(k=h.indexOf("."),0<k)l=h.substring(0,k),k=(e?e:"")+l+".",m||(m=[]),m.push(k),g+=a.l(l,c,d,f,k);else if("boolean"==typeof l&&(l=l?"true":"false"),l){if("retrieveLightData"==f&&0>e.indexOf(".contextData."))switch(k=h.substring(0,4),n=h.substring(4),h){case "transactionID":h=
"xact";break;case "channel":h="ch";break;case "campaign":h="v0";break;default:a.bb(n)&&("prop"==k?h="c"+n:"eVar"==k?h="v"+n:"list"==k?h="l"+n:"hier"==k&&(h="h"+n,l=l.substring(0,255)))}g+="&"+a.escape(h)+"="+a.escape(l)}}""!=g&&(g+="&."+b)}return g};a.usePostbacks=0;a.lc=function(){var b="",c,d,f,e,g,h,l,k,n="",m="",p=e="",r=a.X();if(a.lightProfileID)c=a.Q,(n=a.lightTrackVars)&&(n=","+n+","+a.ra.join(",")+",");else{c=a.i;if(a.pe||a.linkType)n=a.linkTrackVars,m=a.linkTrackEvents,a.pe&&(e=a.pe.substring(0,
1).toUpperCase()+a.pe.substring(1),a[e]&&(n=a[e].yc,m=a[e].xc));n&&(n=","+n+","+a.F.join(",")+",");m&&(m=","+m+",",n&&(n+=",events,"));a.events2&&(p+=(""!=p?",":"")+a.events2)}if(r&&r.getCustomerIDs){e=q;if(g=r.getCustomerIDs())for(d in g)Object.prototype[d]||(f=g[d],"object"==typeof f&&(e||(e={}),f.id&&(e[d+".id"]=f.id),f.authState&&(e[d+".as"]=f.authState)));e&&(b+=a.l("cid",e))}a.AudienceManagement&&a.AudienceManagement.isReady()&&(b+=a.l("d",a.AudienceManagement.getEventCallConfigParams()));for(d=
0;d<c.length;d++){e=c[d];g=a[e];f=e.substring(0,4);h=e.substring(4);g||("events"==e&&p?(g=p,p=""):"marketingCloudOrgID"==e&&r&&a.Y("ECID")&&(g=r.marketingCloudOrgID));if(g&&(!n||0<=n.indexOf(","+e+","))){switch(e){case "customerPerspective":e="cp";break;case "marketingCloudOrgID":e="mcorgid";break;case "supplementalDataID":e="sdid";break;case "timestamp":e="ts";break;case "dynamicVariablePrefix":e="D";break;case "visitorID":e="vid";break;case "marketingCloudVisitorID":e="mid";break;case "analyticsVisitorID":e=
"aid";break;case "audienceManagerLocationHint":e="aamlh";break;case "audienceManagerBlob":e="aamb";break;case "authState":e="as";break;case "pageURL":e="g";255<g.length&&(a.pageURLRest=g.substring(255),g=g.substring(0,255));break;case "pageURLRest":e="-g";break;case "referrer":e="r";break;case "vmk":case "visitorMigrationKey":e="vmt";break;case "visitorMigrationServer":e="vmf";a.ssl&&a.visitorMigrationServerSecure&&(g="");break;case "visitorMigrationServerSecure":e="vmf";!a.ssl&&a.visitorMigrationServer&&
(g="");break;case "charSet":e="ce";break;case "visitorNamespace":e="ns";break;case "cookieDomainPeriods":e="cdp";break;case "cookieLifetime":e="cl";break;case "variableProvider":e="vvp";break;case "currencyCode":e="cc";break;case "channel":e="ch";break;case "transactionID":e="xact";break;case "campaign":e="v0";break;case "latitude":e="lat";break;case "longitude":e="lon";break;case "resolution":e="s";break;case "colorDepth":e="c";break;case "javascriptVersion":e="j";break;case "javaEnabled":e="v";
break;case "cookiesEnabled":e="k";break;case "browserWidth":e="bw";break;case "browserHeight":e="bh";break;case "connectionType":e="ct";break;case "homepage":e="hp";break;case "events":p&&(g+=(""!=g?",":"")+p);if(m)for(h=g.split(","),g="",f=0;f<h.length;f++)l=h[f],k=l.indexOf("="),0<=k&&(l=l.substring(0,k)),k=l.indexOf(":"),0<=k&&(l=l.substring(0,k)),0<=m.indexOf(","+l+",")&&(g+=(g?",":"")+h[f]);break;case "events2":g="";break;case "contextData":b+=a.l("c",a[e],n,e);g="";break;case "clientHints":b=
a.collectHighEntropyUserAgentHints&&a.Aa()?b+a.l("h",a[e],a.mc(n),e):b+a.l("h",a[e],n,e);g="";break;case "lightProfileID":e="mtp";break;case "lightStoreForSeconds":e="mtss";a.lightProfileID||(g="");break;case "lightIncrementBy":e="mti";a.lightProfileID||(g="");break;case "retrieveLightProfiles":e="mtsr";break;case "deleteLightProfiles":e="mtsd";break;case "retrieveLightData":a.retrieveLightProfiles&&(b+=a.l("mts",a[e],n,e));g="";break;default:a.bb(h)&&("prop"==f?e="c"+h:"eVar"==f?e="v"+h:"list"==
f?e="l"+h:"hier"==f&&(e="h"+h,g=g.substring(0,255)))}g&&(b+="&"+e+"="+("pev"!=e.substring(0,3)?a.escape(g):g))}"pev3"==e&&a.e&&(b+=a.e)}a.qa&&(b+="&lrt="+a.qa,a.qa=null);return b};a.C=function(a){var c=a.tagName;if("undefined"!=""+a.Fc||"undefined"!=""+a.tc&&"HTML"!=(""+a.tc).toUpperCase())return"";c=c&&c.toUpperCase?c.toUpperCase():"";"SHAPE"==c&&(c="");c&&(("INPUT"==c||"BUTTON"==c)&&a.type&&a.type.toUpperCase?c=a.type.toUpperCase():!c&&a.href&&(c="A"));return c};a.Ya=function(a){var c=k.location,
d=a.href?a.href:"",f,e,g;"string"!==typeof d&&(d="");f=d.indexOf(":");e=d.indexOf("?");g=d.indexOf("/");d&&(0>f||0<=e&&f>e||0<=g&&f>g)&&(e=a.protocol&&1<a.protocol.length?a.protocol:c.protocol?c.protocol:"",f=c.pathname.lastIndexOf("/"),d=(e?e+"//":"")+(a.host?a.host:c.host?c.host:"")+("/"!=d.substring(0,1)?c.pathname.substring(0,0>f?0:f)+"/":"")+d);return d};a.N=function(b){var c=a.C(b),d,f,e="",g=0;return c&&(d=b.protocol,f=b.onclick,!b.href||"A"!=c&&"AREA"!=c||f&&d&&!(0>d.toLowerCase().indexOf("javascript"))?
f?(e=a.replace(a.replace(a.replace(a.replace(""+f,"\r",""),"\n",""),"\t","")," ",""),g=2):"INPUT"==c||"SUBMIT"==c?(b.value?e=b.value:b.innerText?e=b.innerText:b.textContent&&(e=b.textContent),g=3):"IMAGE"==c&&b.src&&(e=b.src):e=a.Ya(b),e)?{id:e.substring(0,100),type:g}:0};a.Dc=function(b){for(var c=a.C(b),d=a.N(b);b&&!d&&"BODY"!=c;)if(b=b.parentElement?b.parentElement:b.parentNode)c=a.C(b),d=a.N(b);d&&"BODY"!=c||(b=0);b&&(c=b.onclick?""+b.onclick:"",0<=c.indexOf(".tl(")||0<=c.indexOf(".trackLink("))&&
(b=0);return b};a.sc=function(){var b,c,d=a.linkObject,f=a.linkType,e=a.linkURL,g,h;a.ta=1;d||(a.ta=0,d=a.clickObject);if(d){b=a.C(d);for(c=a.N(d);d&&!c&&"BODY"!=b;)if(d=d.parentElement?d.parentElement:d.parentNode)b=a.C(d),c=a.N(d);c&&"BODY"!=b||(d=0);if(d&&!a.linkObject){var l=d.onclick?""+d.onclick:"";if(0<=l.indexOf(".tl(")||0<=l.indexOf(".trackLink("))d=0}}else a.ta=1;!e&&d&&(e=a.Ya(d));e&&!a.linkLeaveQueryString&&(g=e.indexOf("?"),0<=g&&(e=e.substring(0,g)));if(!f&&e){var m=0,n=0,p;if(a.trackDownloadLinks&&
a.linkDownloadFileTypes)for(l=e.toLowerCase(),g=l.indexOf("?"),h=l.indexOf("#"),0<=g?0<=h&&h<g&&(g=h):g=h,0<=g&&(l=l.substring(0,g)),g=a.linkDownloadFileTypes.toLowerCase().split(","),h=0;h<g.length;h++)(p=g[h])&&l.substring(l.length-(p.length+1))=="."+p&&(f="d");if(a.trackExternalLinks&&!f&&(l=e.toLowerCase(),a.ab(l)&&(a.linkInternalFilters||(a.linkInternalFilters=k.location.hostname),g=0,a.linkExternalFilters?(g=a.linkExternalFilters.toLowerCase().split(","),m=1):a.linkInternalFilters&&(g=a.linkInternalFilters.toLowerCase().split(",")),
g))){for(h=0;h<g.length;h++)p=g[h],0<=l.indexOf(p)&&(n=1);n?m&&(f="e"):m||(f="e")}}a.linkObject=d;a.linkURL=e;a.linkType=f;if(a.trackClickMap||a.trackInlineStats)a.e="",d&&(f=a.pageName,e=1,d=d.sourceIndex,f||(f=a.pageURL,e=0),k.s_objectID&&(c.id=k.s_objectID,d=c.type=1),f&&c&&c.id&&b&&(a.e="&pid="+a.escape(f.substring(0,255))+(e?"&pidt="+e:"")+"&oid="+a.escape(c.id.substring(0,100))+(c.type?"&oidt="+c.type:"")+"&ot="+b+(d?"&oi="+d:"")))};a.nc=function(){var b=a.ta,c=a.linkType,d=a.linkURL,f=a.linkName;
c&&(d||f)&&(c=c.toLowerCase(),"d"!=c&&"e"!=c&&(c="o"),a.pe="lnk_"+c,a.decodeLinkParameters?(a.pev1=d?a.unescape(d):"",a.pev2=f?a.unescape(f):"",a.pev1=a.escape(a.pev1),a.pev2=a.escape(a.pev2)):(a.pev1=d?a.escape(d):"",a.pev2=f?a.escape(f):""),b=1);a.abort&&(b=0);if(a.trackClickMap||a.trackInlineStats||a.pc()){var c={},d=0,e=a.Lb(),g=e?e.split("&"):0,h,l,k,e=0;if(g)for(h=0;h<g.length;h++)l=g[h].split("="),f=a.unescape(l[0]).split(","),l=a.unescape(l[1]),c[l]=f;f=a.account.split(",");h={};for(k in a.contextData)k&&
!Object.prototype[k]&&"a.activitymap."==k.substring(0,14)&&(h[k]=a.contextData[k],a.contextData[k]="");a.e=a.l("c",h)+(a.e?a.e:"");if(b||a.e){b&&!a.e&&(e=1);for(l in c)if(!Object.prototype[l])for(k=0;k<f.length;k++)for(e&&(g=c[l].join(","),g==a.account&&(a.e+=("&"!=l.charAt(0)?"&":"")+l,c[l]=[],d=1)),h=0;h<c[l].length;h++)g=c[l][h],g==f[k]&&(e&&(a.e+="&u="+a.escape(g)+("&"!=l.charAt(0)?"&":"")+l+"&u=0"),c[l].splice(h,1),d=1);b||(d=1);if(d){e="";h=2;!b&&a.e&&(e=a.escape(f.join(","))+"="+a.escape(a.e),
h=1);for(l in c)!Object.prototype[l]&&0<h&&0<c[l].length&&(e+=(e?"&":"")+a.escape(c[l].join(","))+"="+a.escape(l),h--);a.Tb(e)}}}return b};a.Lb=function(){if(a.useLinkTrackSessionStorage){if(a.o("sessionStorage"))try{return k.sessionStorage.getItem(a.R)}catch(b){}}else return a.cookieRead(a.R)};a.Tb=function(b){if(a.useLinkTrackSessionStorage){if(a.o("sessionStorage"))try{k.sessionStorage.setItem(a.R,b)}catch(c){}}else a.cookieWrite(a.R,b)};a.oc=function(){if(!a.wc){var b=new Date,c=p.location,d,
f,e=f=d="",g="",h="",l="1.2",k=a.cookieWrite(a.lb,"true",0)?"Y":"N",m="",q="";if(b.setUTCDate&&(l="1.3",(0).toPrecision&&(l="1.5",b=[],b.forEach))){l="1.6";f=0;d={};try{f=new Iterator(d),f.next&&(l="1.7",b.reduce&&(l="1.8",l.trim&&(l="1.8.1",Date.parse&&(l="1.8.2",Object.create&&(l="1.8.5")))))}catch(r){}}d=screen.width+"x"+screen.height;e=navigator.javaEnabled()?"Y":"N";f=screen.pixelDepth?screen.pixelDepth:screen.colorDepth;g=a.w.innerWidth?a.w.innerWidth:a.d.documentElement.offsetWidth;h=a.w.innerHeight?
a.w.innerHeight:a.d.documentElement.offsetHeight;try{a.b.addBehavior("#default#homePage"),m=a.b.Ec(c)?"Y":"N"}catch(s){}try{a.b.addBehavior("#default#clientCaps"),q=a.b.connectionType}catch(t){}a.resolution=d;a.colorDepth=f;a.javascriptVersion=l;a.javaEnabled=e;a.cookiesEnabled=k;a.browserWidth=g;a.browserHeight=h;a.connectionType=q;a.homepage=m;a.wc=1}};a.sb=function(){if(a.collectHighEntropyUserAgentHints&&!a.J&&a.Aa()){a.J=!0;try{navigator.userAgentData.getHighEntropyValues(a.za).then(function(b){a.clientHints=
{};a.za.forEach(function(d){Object.prototype.hasOwnProperty.call(b,d)&&(a.clientHints[d]=b[d])})})["catch"](function(b){a.J=!1;a.clientHints={};a.debugTracking&&a.log(b.message)})}catch(b){a.J=!1,a.clientHints={},a.debugTracking&&a.log(b.message)}}else a.clientHints={}};a.Aa=function(){return"undefined"!==typeof navigator.userAgentData};a.S={};a.loadModule=function(b,c){var d=a.S[b];if(!d){d=k["AppMeasurement_Module_"+b]?new k["AppMeasurement_Module_"+b](a):{};a.S[b]=a[b]=d;d.Cb=function(){return d.Ob};
d.Ub=function(c){if(d.Ob=c)a[b+"_onLoad"]=c,a.ka(b+"_onLoad",[a,d],1)||c(a,d)};try{Object.defineProperty?Object.defineProperty(d,"onLoad",{get:d.Cb,set:d.Ub}):d._olc=1}catch(f){d._olc=1}}c&&(a[b+"_onLoad"]=c,a.ka(b+"_onLoad",[a,d],1)||c(a,d))};a.v=function(b){var c,d;for(c in a.S)if(!Object.prototype[c]&&(d=a.S[c])&&(d._olc&&d.onLoad&&(d._olc=0,d.onLoad(a,d)),d[b]&&d[b]()))return 1;return 0};a.pc=function(){return a.ActivityMap&&a.ActivityMap._c?!0:!1};a.qc=function(){var b=Math.floor(1E13*Math.random()),
c=a.visitorSampling,d=a.visitorSamplingGroup,d="s_vsn_"+(a.visitorNamespace?a.visitorNamespace:a.account)+(d?"_"+d:""),f=a.cookieRead(d);if(c){c*=100;f&&(f=parseInt(f));if(!f){if(!a.cookieWrite(d,b))return 0;f=b}if(f%1E4>c)return 0}return 1};a.U=function(b,c){var d,f,e,g,h,l,k;k={};for(d=0;2>d;d++)for(f=0<d?a.Ra:a.i,e=0;e<f.length;e++)if(g=f[e],(h=b[g])||b["!"+g]){if(h&&!c&&("contextData"==g||"retrieveLightData"==g)&&a[g])for(l in a[g])h[l]||(h[l]=a[g][l]);a[g]||(k["!"+g]=1);k[g]=a[g];a[g]=h}return k};
a.Bc=function(b){var c,d,f,e;for(c=0;2>c;c++)for(d=0<c?a.Ra:a.i,f=0;f<d.length;f++)e=d[f],b[e]=a[e],b[e]||"prop"!==e.substring(0,4)&&"eVar"!==e.substring(0,4)&&"hier"!==e.substring(0,4)&&"list"!==e.substring(0,4)&&"channel"!==e&&"events"!==e&&"eventList"!==e&&"products"!==e&&"productList"!==e&&"purchaseID"!==e&&"transactionID"!==e&&"state"!==e&&"zip"!==e&&"campaign"!==e&&"events2"!==e&&"latitude"!==e&&"longitude"!==e&&"ms_a"!==e&&"contextData"!==e&&"supplementalDataID"!==e&&"tnt"!==e&&"timestamp"!==
e&&"abort"!==e&&"useBeacon"!==e&&"linkObject"!==e&&"clickObject"!==e&&"linkType"!==e&&"linkName"!==e&&"linkURL"!==e&&"bodyClickTarget"!==e&&"bodyClickFunction"!==e||(b["!"+e]=1)};a.ic=function(a){var c,d,f,e,g,h=0,k,m="",n="";if(a&&255<a.length&&(c=""+a,d=c.indexOf("?"),0<d&&(k=c.substring(d+1),c=c.substring(0,d),e=c.toLowerCase(),f=0,"http://"==e.substring(0,7)?f+=7:"https://"==e.substring(0,8)&&(f+=8),d=e.indexOf("/",f),0<d&&(e=e.substring(f,d),g=c.substring(d),c=c.substring(0,d),0<=e.indexOf("google")?
h=",q,ie,start,search_key,word,kw,cd,":0<=e.indexOf("yahoo.co")?h=",p,ei,":0<=e.indexOf("baidu.")&&(h=",wd,word,"),h&&k)))){if((a=k.split("&"))&&1<a.length){for(f=0;f<a.length;f++)e=a[f],d=e.indexOf("="),0<d&&0<=h.indexOf(","+e.substring(0,d)+",")?m+=(m?"&":"")+e:n+=(n?"&":"")+e;m&&n?k=m+"&"+n:n=""}d=253-(k.length-n.length)-c.length;a=c+(0<d?g.substring(0,d):"")+"?"+k}return a};a.qb=function(b){var c=a.d.visibilityState,d=["webkitvisibilitychange","visibilitychange"];c||(c=a.d.webkitVisibilityState);
if(c&&"prerender"==c){if(b)for(c=0;c<d.length;c++)a.d.addEventListener(d[c],function(){var c=a.d.visibilityState;c||(c=a.d.webkitVisibilityState);"visible"==c&&b()});return!1}return!0};a.ha=!1;a.H=!1;a.Xb=function(){a.H=!0;a.q()};a.K=!1;a.Yb=function(b){a.marketingCloudVisitorID=b.MCMID;a.visitorOptedOut=b.MCOPTOUT;a.analyticsVisitorID=b.MCAID;a.audienceManagerLocationHint=b.MCAAMLH;a.audienceManagerBlob=b.MCAAMB;a.K=!1;a.q()};a.pb=function(b){a.maxDelay||(a.maxDelay=250);return a.v("_d")?(b&&setTimeout(function(){b()},
a.maxDelay),!1):!0};a.fa=!1;a.G=!1;a.Ka=function(){a.G=!0;a.q()};a.isReadyToTrack=function(){var b=!0;if(!a.Hb()||!a.Fb())return!1;a.Jb()||(b=!1);a.Nb()||(b=!1);a.rb()||(b=!1);return b};a.Hb=function(){a.ha||a.H||(a.qb(a.Xb)?a.H=!0:a.ha=!0);return a.ha&&!a.H?!1:!0};a.Fb=function(){var b=a.Ga();if(b)if(a.Ca||a.ga)if(a.Ca){if(!b.isApproved(b.Categories.ANALYTICS))return!1}else return!1;else return b.fetchPermissions(a.Pb,!0),a.ga=!0,!1;return!0};a.Y=function(b){var c=a.Ga();return c&&!c.isApproved(c.Categories[b])?
!1:!0};a.Ga=function(){return k.adobe&&k.adobe.optIn?k.adobe.optIn:null};a.da=!0;a.Jb=function(){var b=a.X();if(!b||!b.getVisitorValues)return!0;a.da&&(a.da=!1,a.K||(a.K=!0,b.getVisitorValues(a.Yb)));return!a.K};a.X=function(){var b=a.visitor;b&&!b.isAllowed()&&(b=null);return b};a.Nb=function(){a.fa||a.G||(a.pb(a.Ka)?a.G=!0:a.fa=!0);return a.fa&&!a.G?!1:!0};a.rb=function(){a.J||a.clientHints||a.sb();return a.clientHints};a.ga=!1;a.Pb=function(){a.ga=!1;a.Ca=!0};a.j=q;a.r=0;a.callbackWhenReadyToTrack=
function(b,c,d){var f;f={};f.bc=b;f.ac=c;f.Zb=d;a.j==q&&(a.j=[]);a.j.push(f);0==a.r&&(a.r=setInterval(a.q,100))};a.q=function(){var b;if(a.isReadyToTrack()&&(a.Vb(),a.j!=q))for(;0<a.j.length;)b=a.j.shift(),b.ac.apply(b.bc,b.Zb)};a.Vb=function(){a.r&&(clearInterval(a.r),a.r=0)};a.Da=function(b){var c,d={};a.Bc(d);if(b!=q)for(c in b)d[c]=b[c];a.callbackWhenReadyToTrack(a,a.Qa,[d]);a.Na()};a.jc=function(){var b=a.cookieRead("s_fid"),c="",d="",f;f=8;var e=4;if(!b||0>b.indexOf("-")){for(b=0;16>b;b++)f=
Math.floor(Math.random()*f),c+="0123456789ABCDEF".substring(f,f+1),f=Math.floor(Math.random()*e),d+="0123456789ABCDEF".substring(f,f+1),f=e=16;b=c+"-"+d}a.cookieWrite("s_fid",b,1)||(b=0);return b};a.Qa=function(b){var c=new Date,d="s"+Math.floor(c.getTime()/108E5)%10+Math.floor(1E13*Math.random()),f=c.getYear(),f="t="+a.escape(c.getDate()+"/"+c.getMonth()+"/"+(1900>f?f+1900:f)+" "+c.getHours()+":"+c.getMinutes()+":"+c.getSeconds()+" "+c.getDay()+" "+c.getTimezoneOffset()),e=a.X(),g;b&&(g=a.U(b,1));
a.qc()&&!a.visitorOptedOut&&(a.Ha()||(a.fid=a.jc()),a.sc(),a.usePlugins&&a.doPlugins&&a.doPlugins(a),a.account&&(a.abort||(a.trackOffline&&!a.timestamp&&(a.timestamp=Math.floor(c.getTime()/1E3)),b=k.location,a.pageURL||(a.pageURL=b.href?b.href:b),a.referrer||a.mb||(b=a.Util.getQueryParam("adobe_mc_ref",null,null,!0),a.referrer=b||void 0===b?void 0===b?"":b:p.document.referrer),a.mb=1,!a.referrer&&a.ea&&(a.referrer=a.ea),a.ea=0,a.referrer=a.ic(a.referrer),a.v("_g")),a.nc()&&!a.abort&&(e&&a.Y("TARGET")&&
!a.supplementalDataID&&e.getSupplementalDataID&&(a.supplementalDataID=e.getSupplementalDataID("AppMeasurement:"+a._in,a.expectSupplementalData?!1:!0)),a.Y("AAM")||(a.contextData["cm.ssf"]=1),a.oc(),a.Qb(),f+=a.lc(),a.Mb(d,f),a.v("_t"),a.referrer="",a.contextData&&a.contextData.excCodes&&(a.contextData.excCodes=0))));a.referrer&&(a.ea=a.referrer);a.Na();g&&a.U(g,1)};a.t=a.track=function(b,c){c&&a.U(c);a.da=!0;a.isReadyToTrack()?null!=a.j&&0<a.j.length?(a.Da(b),a.q()):a.Qa(b):a.Da(b)};a.Qb=function(){a.writeSecureCookies&&
!a.ssl&&a.nb()};a.nb=function(){a.contextData.excCodes=a.contextData.excCodes||[];a.contextData.excCodes.push(1)};a.Na=function(){a.abort=a.supplementalDataID=a.timestamp=a.pageURLRest=a.linkObject=a.clickObject=a.linkURL=a.linkName=a.linkType=k.s_objectID=a.pe=a.pev1=a.pev2=a.pev3=a.e=a.lightProfileID=a.useBeacon=a.referrer=0};a.Ma=[];a.registerPreTrackCallback=function(b){for(var c=[],d=1;d<arguments.length;d++)c.push(arguments[d]);"function"==typeof b?a.Ma.push([b,c]):a.debugTracking&&a.log("Warning, Non function type passed to registerPreTrackCallback")};
a.xb=function(b){a.Fa(a.Ma,b)};a.La=[];a.registerPostTrackCallback=function(b){for(var c=[],d=1;d<arguments.length;d++)c.push(arguments[d]);"function"==typeof b?a.La.push([b,c]):a.debugTracking&&a.log("Warning, Non function type passed to registerPostTrackCallback")};a.wb=function(b){a.Fa(a.La,b)};a.Fa=function(b,c){if("object"==typeof b)for(var d=0;d<b.length;d++){var f=b[d][0],e=b[d][1].slice();e.unshift(c);if("function"==typeof f)try{f.apply(null,e)}catch(g){a.debugTracking&&a.log(g.message)}}};
a.tl=a.trackLink=function(b,c,d,f,e){a.linkObject=b;a.linkType=c;a.linkName=d;e&&(a.bodyClickTarget=b,a.bodyClickFunction=e);return a.track(f)};a.trackLight=function(b,c,d,f){a.lightProfileID=b;a.lightStoreForSeconds=c;a.lightIncrementBy=d;return a.track(f)};a.clearVars=function(){var b,c;for(b=0;b<a.i.length;b++)if(c=a.i[b],"prop"==c.substring(0,4)||"eVar"==c.substring(0,4)||"hier"==c.substring(0,4)||"list"==c.substring(0,4)||"channel"==c||"events"==c||"eventList"==c||"products"==c||"productList"==
c||"purchaseID"==c||"transactionID"==c||"state"==c||"zip"==c||"campaign"==c)a[c]=void 0};a.tagContainerMarker="";a.Mb=function(b,c){var d=a.Ab()+"/"+b+"?AQB=1&ndh=1&pf=1&"+(a.Ja()?"callback=s_c_il["+a._in+"].doPostbacks&et=1&":"")+c+"&AQE=1";a.xb(d);a.V?a.Wb(d):(a.Oa(),a.Ea(d),a.I())};a.Ab=function(){var b=a.Bb();return"http"+(a.ssl?"s":"")+"://"+b+"/b/ss/"+a.account+"/"+(a.mobile?"5.":"")+(a.Ja()?"10":"1")+"/JS-"+a.version+(a.vc?"T":"")+(a.tagContainerMarker?"-"+a.tagContainerMarker:"")};a.Ja=function(){return a.AudienceManagement&&
a.AudienceManagement.isReady()||0!=a.usePostbacks};a.Bb=function(){var b=a.dc,c=a.trackingServer;c?a.trackingServerSecure&&a.ssl&&(c=a.trackingServerSecure):(b=b?(""+b).toLowerCase():"d1","d1"==b?b="112":"d2"==b&&(b="122"),c=a.Db()+"."+b+".2o7.net");return c};a.Db=function(){var b=a.visitorNamespace;b||(b=a.account.split(",")[0],b=b.replace(/[^0-9a-z]/gi,""));return b};a.kb=/{(%?)(.*?)(%?)}/;a.Ac=RegExp(a.kb.source,"g");a.hc=function(b){if("object"==typeof b.dests)for(var c=0;c<b.dests.length;++c){var d=
b.dests[c];if("string"==typeof d.c&&"aa."==d.id.substr(0,3))for(var f=d.c.match(a.Ac),e=0;e<f.length;++e){var g=f[e],h=g.match(a.kb),k="";"%"==h[1]&&"timezone_offset"==h[2]?k=(new Date).getTimezoneOffset():"%"==h[1]&&"timestampz"==h[2]&&(k=a.kc());d.c=d.c.replace(g,a.escape(k))}}};a.kc=function(){var b=new Date,c=new Date(6E4*Math.abs(b.getTimezoneOffset()));return a.k(4,b.getFullYear())+"-"+a.k(2,b.getMonth()+1)+"-"+a.k(2,b.getDate())+"T"+a.k(2,b.getHours())+":"+a.k(2,b.getMinutes())+":"+a.k(2,b.getSeconds())+
(0<b.getTimezoneOffset()?"-":"+")+a.k(2,c.getUTCHours())+":"+a.k(2,c.getUTCMinutes())};a.k=function(a,c){return(Array(a+1).join(0)+c).slice(-a)};a.wa={};a.doPostbacks=function(b){if("object"==typeof b)if(a.hc(b),"object"==typeof a.AudienceManagement&&"function"==typeof a.AudienceManagement.isReady&&a.AudienceManagement.isReady()&&"function"==typeof a.AudienceManagement.passData)a.AudienceManagement.passData(b);else if("object"==typeof b&&"object"==typeof b.dests)for(var c=0;c<b.dests.length;++c){var d=
b.dests[c];"object"==typeof d&&"string"==typeof d.c&&"string"==typeof d.id&&"aa."==d.id.substr(0,3)&&(a.wa[d.id]=new Image,a.wa[d.id].alt="",a.wa[d.id].src=d.c)}};a.bufferRequests=function(b){b||void 0===b?a.ub():a.tb()};a.ub=function(){a.o("sessionStorage")?a.V=!0:a.debugTracking&&a.log("Warning, unable to access session-storage requests will not be buffered.")};a.tb=function(){a.V&&a.Oa();a.V=!1};a.o=function(b){var c=!1,d;try{(d=a.w[b])&&d.setItem&&k.JSON&&(c=!0)}catch(f){a.debugTracking&&a.log("Warning, "+
b+" is not available, "+f.message)}return c};a.Oa=function(){var b=a.ba("sessionStorage");if(b){for(var c=0;c<b.length;c++)a.Ea(b[c]);a.Ba("sessionStorage");a.I()}};a.Ea=function(b){a.g||a.Eb();a.g.push(b);a.pa=a.B();a.jb()};a.Wb=function(b){var c=a.ba("sessionStorage")||[];c.push(b);a.Pa("sessionStorage",c)};a.Pa=function(b,c){try{k[b].setItem(a.ca(),k.JSON.stringify(c))}catch(d){}};a.ba=function(b){var c,d;if(a.o(b)){try{(d=k[b].getItem(a.ca()))&&(c=k.JSON.parse(d))}catch(f){}return c}};a.Eb=function(){a.va()&&
(a.g=a.ba("localStorage"));a.g||(a.g=[])};a.Ba=function(b){if(a.o(b))try{k[b].removeItem(a.ca())}catch(c){}};a.va=function(){var b=!0;a.trackOffline&&a.storageFilename&&a.o("localStorage")||(b=!1);return b};a.Za=function(){var b=0;a.g&&(b=a.g.length);a.p&&b++;return b};a.mc=function(b){var c=a.clientHints||{},d="",f;for(f in c)d+="clientHints."+f+",";return b?b+d:b};a.I=function(){if(a.p&&(a.A&&a.A.complete&&a.A.D&&a.A.T(),a.p))return;a.$a=q;if(a.ua)a.pa>a.P&&a.hb(a.g),a.xa(500);else{var b=a.$b();
if(0<b)a.xa(b);else if(b=a.Xa())a.p=1,a.rc(b),a.uc(b)}};a.xa=function(b){a.$a||(b||(b=0),a.$a=setTimeout(a.I,b))};a.$b=function(){var b;if(!a.trackOffline||0>=a.offlineThrottleDelay)return 0;b=a.B()-a.fb;return a.offlineThrottleDelay<b?0:a.offlineThrottleDelay-b};a.Xa=function(){if(a.g&&0<a.g.length)return a.g.shift()};a.rc=function(b){if(a.debugTracking){var c="AppMeasurement Debug: "+b;b=b.split("&");var d;for(d=0;d<b.length;d++)c+="\n\t"+a.unescape(b[d]);a.log(c)}};a.Ha=function(){return a.marketingCloudVisitorID||
a.analyticsVisitorID};a.aa=!1;var u;try{u=JSON.parse('{"x":"y"}')}catch(x){u=null}u&&"y"==u.x?(a.aa=!0,a.Z=function(a){return JSON.parse(a)}):k.$&&k.$.parseJSON?(a.Z=function(a){return k.$.parseJSON(a)},a.aa=!0):a.Z=function(){return null};a.uc=function(b){var c,d,f;a.Gb(b)&&(d=1,c={send:function(b){a.useBeacon=!1;navigator.sendBeacon(b)?c.T():c.na()}});!c&&a.Ha()&&2047<b.length&&(a.ob()&&(d=2,c=new XMLHttpRequest),c&&(a.AudienceManagement&&a.AudienceManagement.isReady()||0!=a.usePostbacks)&&(a.aa?
c.Sa=!0:c=0));!c&&a.zc&&(b=b.substring(0,2047));!c&&a.d.createElement&&(0!=a.usePostbacks||a.AudienceManagement&&a.AudienceManagement.isReady())&&(c=a.d.createElement("SCRIPT"))&&"async"in c&&((f=(f=a.d.getElementsByTagName("HEAD"))&&f[0]?f[0]:a.d.body)?(c.type="text/javascript",c.setAttribute("async","async"),d=3):c=0);c||(c=new Image,d=4,c.alt="",c.abort||"undefined"===typeof k.InstallTrigger||(c.abort=function(){c.src=q}));c.gb=Date.now();c.Ua=function(){try{c.D&&(clearTimeout(c.D),c.D=0)}catch(a){}};
c.onload=c.T=function(){if(!0!==c.cc&&(c.cc=!0,c.gb&&(a.qa=Date.now()-c.gb),a.wb(b),c.Ua(),a.fc(),a.ia(),a.p=0,a.I(),c.Sa)){c.Sa=!1;try{a.doPostbacks(a.Z(c.responseText))}catch(d){}}};c.onabort=c.onerror=c.na=function(){c.Ua();(a.trackOffline||a.ua)&&a.p&&a.g.unshift(a.ec);a.p=0;a.pa>a.P&&a.hb(a.g);a.ia();a.xa(500)};c.onreadystatechange=function(){4==c.readyState&&(200==c.status?c.T():c.na())};a.fb=a.B();if(1===d)c.send(b);else if(2===d)f=b.indexOf("?"),d=b.substring(0,f),f=b.substring(f+1),f=f.replace(/&callback=[a-zA-Z0-9_.\[\]]+/,
""),c.open("POST",d,!0),c.withCredentials=!0,c.setRequestHeader("Content-Type","application/x-www-form-urlencoded"),c.send(f);else if(c.src=b,3===d){if(a.cb)try{f.removeChild(a.cb)}catch(e){}f.firstChild?f.insertBefore(c,f.firstChild):f.appendChild(c);a.cb=a.A}c.D=setTimeout(function(){c.D&&(c.complete?c.T():(a.trackOffline&&c.abort&&c.abort(),c.na()))},5E3);a.ec=b;a.A=k["s_i_"+a.replace(a.account,",","_")]=c;if(a.useForcedLinkTracking&&a.L||a.bodyClickFunction)a.forcedLinkTrackingTimeout||(a.forcedLinkTrackingTimeout=
250),a.ja=setTimeout(a.ia,a.forcedLinkTrackingTimeout)};a.Gb=function(b){var c=!1;navigator.sendBeacon&&(a.Ib(b)?c=!0:a.useBeacon&&(c=!0));a.Sb(b)&&(c=!1);return c};a.Ib=function(a){return a&&0<a.indexOf("pe=lnk_e")?!0:!1};a.Sb=function(a){return 64E3<=a.length};a.ob=function(){return"undefined"!==typeof XMLHttpRequest&&"withCredentials"in new XMLHttpRequest?!0:!1};a.fc=function(){!a.va()||a.eb>a.P||(a.Ba("localStorage"),a.eb=a.B())};a.hb=function(b){a.va()&&(a.jb(),a.Pa("localStorage",b),a.P=a.B())};
a.jb=function(){if(a.trackOffline){if(!a.offlineLimit||0>=a.offlineLimit)a.offlineLimit=10;for(;a.g.length>a.offlineLimit;)a.Xa()}};a.forceOffline=function(){a.ua=!0};a.forceOnline=function(){a.ua=!1};a.ca=function(){return a.storageFilename+"-"+a.visitorNamespace+a.account};a.B=function(){return(new Date).getTime()};a.ab=function(a){a=a.toLowerCase();return 0!=a.indexOf("#")&&0!=a.indexOf("about:")&&0!=a.indexOf("opera:")&&0!=a.indexOf("javascript:")?!0:!1};a.setTagContainer=function(b){var c,d,
f;a.vc=b;for(c=0;c<a._il.length;c++)if((d=a._il[c])&&"s_l"==d._c&&d.tagContainerName==b){a.U(d);if(d.lmq)for(c=0;c<d.lmq.length;c++)f=d.lmq[c],a.loadModule(f.n);if(d.ml)for(f in d.ml)if(a[f])for(c in b=a[f],f=d.ml[f],f)!Object.prototype[c]&&("function"!=typeof f[c]||0>(""+f[c]).indexOf("s_c_il"))&&(b[c]=f[c]);if(d.mmq)for(c=0;c<d.mmq.length;c++)f=d.mmq[c],a[f.m]&&(b=a[f.m],b[f.f]&&"function"==typeof b[f.f]&&(f.a?b[f.f].apply(b,f.a):b[f.f].apply(b)));if(d.tq)for(c=0;c<d.tq.length;c++)a.track(d.tq[c]);
d.s=a;break}};a.Util={urlEncode:a.escape,urlDecode:a.unescape,cookieRead:a.cookieRead,cookieWrite:a.cookieWrite,getQueryParam:function(b,c,d,f){var e,g="";c||(c=a.pageURL?a.pageURL:k.location);d=d?d:"&";if(!b||!c)return g;c=""+c;e=c.indexOf("?");if(0>e)return g;c=d+c.substring(e+1)+d;if(!f||!(0<=c.indexOf(d+b+d)||0<=c.indexOf(d+b+"="+d))){e=c.indexOf("#");0<=e&&(c=c.substr(0,e)+d);e=c.indexOf(d+b+"=");if(0>e)return g;c=c.substring(e+d.length+b.length+1);e=c.indexOf(d);0<=e&&(c=c.substring(0,e));0<
c.length&&(g=a.unescape(c));return g}},getIeVersion:function(){return document.documentMode?document.documentMode:a.Ia()?7:null}};a.F="supplementalDataID timestamp dynamicVariablePrefix visitorID marketingCloudVisitorID analyticsVisitorID audienceManagerLocationHint authState fid vmk visitorMigrationKey visitorMigrationServer visitorMigrationServerSecure charSet visitorNamespace cookieDomainPeriods fpCookieDomainPeriods cookieLifetime pageName pageURL customerPerspective referrer contextData contextData.cm.ssf contextData.opt.dmp contextData.opt.sell clientHints currencyCode lightProfileID lightStoreForSeconds lightIncrementBy retrieveLightProfiles deleteLightProfiles retrieveLightData".split(" ");
a.i=a.F.concat("purchaseID variableProvider channel server pageType transactionID campaign state zip events events2 products audienceManagerBlob tnt".split(" "));a.ra="timestamp charSet visitorNamespace cookieDomainPeriods cookieLifetime contextData lightProfileID lightStoreForSeconds lightIncrementBy".split(" ");a.Q=a.ra.slice(0);a.Ra="account allAccounts debugTracking visitor visitorOptedOut trackOffline offlineLimit offlineThrottleDelay storageFilename usePlugins doPlugins configURL visitorSampling visitorSamplingGroup linkObject clickObject linkURL linkName linkType trackDownloadLinks trackExternalLinks trackClickMap trackInlineStats linkLeaveQueryString linkTrackVars linkTrackEvents linkDownloadFileTypes linkExternalFilters linkInternalFilters useForcedLinkTracking forcedLinkTrackingTimeout writeSecureCookies decodeLinkParameters useLinkTrackSessionStorage collectHighEntropyUserAgentHints trackingServer trackingServerSecure ssl abort mobile dc lightTrackVars maxDelay expectSupplementalData useBeacon usePostbacks registerPreTrackCallback registerPostTrackCallback bodyClickTarget bodyClickFunction bufferRequests AudienceManagement".split(" ");
for(m=0;250>=m;m++)76>m&&(a.i.push("prop"+m),a.Q.push("prop"+m)),a.i.push("eVar"+m),a.Q.push("eVar"+m),6>m&&a.i.push("hier"+m),4>m&&a.i.push("list"+m);m="pe pev1 pev2 pev3 latitude longitude resolution colorDepth javascriptVersion javaEnabled cookiesEnabled browserWidth browserHeight connectionType homepage pageURLRest marketingCloudOrgID ms_a".split(" ");a.i=a.i.concat(m);a.F=a.F.concat(m);a.ssl=0<=k.location.protocol.toLowerCase().indexOf("https");a.charSet="UTF-8";a.contextData={};a.za=["architecture",
"bitness","model","platformVersion","wow64"];a.writeSecureCookies=!1;a.collectHighEntropyUserAgentHints=!1;a.offlineThrottleDelay=0;a.storageFilename="AppMeasurement.requests";a.R="s_sq";a.fb=0;a.pa=0;a.P=0;a.eb=0;a.linkDownloadFileTypes="avi,dmg,doc,docx,exe,extz,gz,m4a,mov,mp3,mpg,pdf,pkg,ppt,pptx,rss,safariextz,sit,wav,wmv,xls,xlsx,xml,zip";a.lb="s_cc";a.w=k;a.d=k.document;a.ia=function(){a.ja&&(k.clearTimeout(a.ja),a.ja=q);a.bodyClickTarget&&a.L&&a.bodyClickTarget.dispatchEvent(a.L);a.bodyClickFunction&&("function"==typeof a.bodyClickFunction?
a.bodyClickFunction():a.bodyClickTarget&&a.bodyClickTarget.href&&(a.d.location=a.bodyClickTarget.href));a.bodyClickTarget=a.L=a.bodyClickFunction=0};a.ib=function(){a.b=a.d.body;a.b?(a.u=function(b){var c,d,f,e,g;if(!(a.d&&a.d.getElementById("cppXYctnr")||b&&b["s_fe_"+a._in])){if(a.Ta)if(a.useForcedLinkTracking)a.b.removeEventListener("click",a.u,!1);else{a.b.removeEventListener("click",a.u,!0);a.Ta=a.useForcedLinkTracking=0;return}else a.useForcedLinkTracking=0;a.clickObject=b.srcElement?b.srcElement:
b.target;try{if(!a.clickObject||a.O&&a.O==a.clickObject||!(a.clickObject.tagName||a.clickObject.parentElement||a.clickObject.parentNode))a.clickObject=0;else{var h=a.O=a.clickObject;a.oa&&(clearTimeout(a.oa),a.oa=0);a.oa=setTimeout(function(){a.O==h&&(a.O=0)},1E4);f=a.Za();a.track();if(f<a.Za()&&a.useForcedLinkTracking&&b.target){for(e=b.target;e&&e!=a.b&&"A"!=e.tagName.toUpperCase()&&"AREA"!=e.tagName.toUpperCase();)e=e.parentNode;if(e&&(g=e.href,a.ab(g)||(g=0),d=e.target,b.target.dispatchEvent&&
g&&(!d||"_self"==d||"_top"==d||"_parent"==d||k.name&&d==k.name))){try{c=a.d.createEvent("MouseEvents")}catch(l){c=new k.MouseEvent}if(c){try{c.initMouseEvent("click",b.bubbles,b.cancelable,b.view,b.detail,b.screenX,b.screenY,b.clientX,b.clientY,b.ctrlKey,b.altKey,b.shiftKey,b.metaKey,b.button,b.relatedTarget)}catch(m){c=0}c&&(c["s_fe_"+a._in]=c.s_fe=1,b.stopPropagation(),b.stopImmediatePropagation&&b.stopImmediatePropagation(),b.preventDefault(),a.bodyClickTarget=b.target,a.L=c)}}}}}catch(n){a.clickObject=
0}}},a.b&&a.b.attachEvent?a.b.attachEvent("onclick",a.u):a.b&&a.b.addEventListener&&(navigator&&(0<=navigator.userAgent.indexOf("WebKit")&&a.d.createEvent||0<=navigator.userAgent.indexOf("Firefox/2")&&k.MouseEvent)&&(a.Ta=1,a.useForcedLinkTracking=1,a.b.addEventListener("click",a.u,!0)),a.b.addEventListener("click",a.u,!1))):setTimeout(a.ib,30)};a.zc=a.Ia();a.gc();a.Gc||(s?a.setAccount(s):a.log("Error, missing Report Suite ID in AppMeasurement initialization"),a.ib(),a.loadModule("ActivityMap"))}
function s_gi(s){var a,k=window.s_c_il,m,q,v=s.split(","),p,r,t=0;if(k)for(m=0;!t&&m<k.length;){a=k[m];if("s_c"==a._c&&(a.account||a.oun))if(a.account&&a.account==s)t=1;else for(q=a.account?a.account:a.oun,q=a.allAccounts?a.allAccounts:q.split(","),p=0;p<v.length;p++)for(r=0;r<q.length;r++)v[p]==q[r]&&(t=1);m++}t?a.setAccount&&a.setAccount(s):a=new AppMeasurement(s);return a}AppMeasurement.getInstance=s_gi;window.s_objectID||(window.s_objectID=0);
function s_pgicq(){var s=window,a=s.s_giq,k,m,q;if(a)for(k=0;k<a.length;k++)m=a[k],q=s_gi(m.oun),q.setAccount(m.un),q.setTagContainer(m.tagContainerName);s.s_giq=0}s_pgicq();
