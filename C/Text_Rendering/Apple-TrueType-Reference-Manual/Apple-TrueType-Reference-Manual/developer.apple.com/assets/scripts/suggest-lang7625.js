// window.addEventListener('DOMContentLoaded', () => {
((window, undefined) => {
  var storage = getStorage();

  var limit = {
    local: 2,
    session: 1,
  };

  const LanguageCallToAction = {
    en: "View in English",
    zh_CN: "查看简体中文页面",
    zh_TW: "檢視繁體中文版",
    ja_JP: "このページを日本語で見る",
    ko_KR: "한국어로 보기",
    de_DE: "Auf Deutsch anzeigen",
    es_lamr: "Ver en español latinoamericano",
    fr_FR: "Afficher la version française",
    it_IT: "Vedi la versione italiana",
    pt_BR: "Veja a versão em português (Brasil)",
    bg_BG: "Преглед на български",
    el_GR: "Προβολή στα ελληνικά",
    cs_CZ: "Zobrazit v češtině",
    da_DK: "Vis på dansk",
    et_EE: "Vaata eesti keeles",
    en_GB: "View in English (UK)",
    es_ES: "Ver en español (España)",
    en_IE: "View in English (Ireland)",
    hr_HR: "Pregled na hrvatskom",
    lv_LV: "Skatīt latviešu valodā",
    lt_LT: "Žiūrėti lietuvių kalba",
    ro_RO: "Vezi în română",
    hu_HU: "Megtekintés magyarul",
    mt_MT: "Ara bil-Malti",
    nl_NL: "Bekijk in het Nederlands",
    pl_PL: "Zobacz w języku polskim",
    pt_PT: "Ver em português (Portugal)",
    sk_SK: "Zobraziť v slovenčine",
    sl_SI: "Ogled v slovenščini",
    fi_FI: "Näytä suomeksi",
    sv_SE: "Visa på svenska",
  };

  const LanguageDismiss = {
    en: "Dismiss language suggestion",
    zh_CN: "关闭语言建议",
    zh_TW: "關閉語言建議",
    ja_JP: "表示言語の提案を閉じる",
    ko_KR: "언어 제안 닫기",
    de_DE: "Sprachvorschlag schließen",
    es_lamr: "Descartar sugerencia de idioma",
    fr_FR: "Ignorer la suggestion de langue",
    it_IT: "Ignora suggerimento lingua",
    pt_BR: "Ignorar sugestão de idioma",
    bg_BG: "Отхвърляне на предложения език",
    el_GR: "Παράβλεψη πρότασης γλώσσας",
    cs_CZ: "Zavřít návrh jazyka",
    da_DK: "Afvis sprogforslag",
    et_EE: "Loobu keelesoovitusest",
    en_GB: "Dismiss language suggestion",
    es_ES: "Descartar sugerencia de idioma",
    en_IE: "Dismiss language suggestion",
    hr_HR: "Odbaci prijedlog jezika",
    lv_LV: "Nerādīt valodas ieteikumu",
    lt_LT: "Atsisakyti kalbos pasiūlymo",
    ro_RO: "Ignoră sugestia de limbă",
    hu_HU: "Nyelvi javaslat elvetése",
    mt_MT: "Injora s-suġġeriment tal-lingwa",
    nl_NL: "Taalvoorstel negeren",
    pl_PL: "Zamknij sugestię językową",
    pt_PT: "Ignorar sugestão de idioma",
    sk_SK: "Zavrieť návrh jazyka",
    sl_SI: "Opusti predlog jezika",
    fi_FI: "Hylkää kieliehdotus",
    sv_SE: "Avvisa språkförslag",
  };

  // Cache values from the page

  const browserLang = navigator.language.toLowerCase();

  const pageLang = document.body.parentElement.lang.replace("-", "_");

  const alternates = Array.from(
    document.querySelectorAll("link[rel='alternate'][hreflang]"),
  );

  const suggestLang = document.getElementById("suggest-lang");
  const suggestLink = document.getElementById("suggest-link");
  const suggestCloser = document.getElementById("suggest-closer");

  if (
    browserLang != pageLang &&
    storage.local < limit.local &&
    storage.session < limit.session
  ) {
    const languages = Object.keys(window.LanguageLocales);

    languages.map((language) => {
      const locales = window.LanguageLocales[language];

      if (locales.includes(browserLang) && language !== pageLang) {
        alternates.map((alternate) => {
          if (alternate.hreflang.replace("-", "_") == language) {
            if (suggestLink) {
              suggestLink.href = alternate.href;
              suggestLink.text = LanguageCallToAction[language];
            }
            if (suggestCloser) {
              suggestCloser.ariaLabel = LanguageDismiss[language];
            }
            if (suggestLang) {
              suggestLang.lang = alternate.hreflang;
              suggestLang.classList.remove("hide");
            }
          }
        });
      }
    });
  }

  if (suggestCloser) {
    suggestCloser.onclick = () => {
      storage = getStorage();

      storage.local++;
      storage.session++;

      localStorage.setItem("suggestLang", storage.local);
      sessionStorage.setItem("suggestLang", storage.session);

      suggestLang.classList.add("hide");
    };
  }

  function getStorage() {
    var local = 0;
    var session = 0;

    if (localStorage.hasOwnProperty("suggestLang")) {
      if (!isNaN(parseInt(localStorage.getItem("suggestLang")))) {
        local = localStorage.getItem("suggestLang");
      }
    }

    if (sessionStorage.hasOwnProperty("suggestLang")) {
      if (!isNaN(parseInt(sessionStorage.getItem("suggestLang")))) {
        session = sessionStorage.getItem("suggestLang");
      }
    }

    return {
      local,
      session,
    };
  }

  // });
})(window);
