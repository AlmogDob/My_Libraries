// Inlines localized strings in 9 supported languages.
const strings = {
  // German
  de_DE: {
    authenticated: {
      userInformation: "Benutzerinformationen",
      accountActions: "Accountaktionen",
      accountOverview: "Account",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "Zertifikate, Kennungen und Profile (Englisch)",
      viewEvents: "Events (Englisch)",
      feedbackAssistant: "Feedback-Assistent",
      forums: "Foren (Englisch)",
      signOut: "Abmelden",
    },
    unauthenticated: {
      signIn: "Anmelden",
    },
  },
  // English
  en_US: {
    authenticated: {
      userInformation: "User information",
      accountActions: "Account actions",
      accountOverview: "Account",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "Certificates, IDs & Profiles",
      viewEvents: "Events",
      feedbackAssistant: "Feedback Assistant",
      forums: "Forums",
      signOut: "Sign out",
    },
    unauthenticated: {
      signIn: "Sign in",
    },
  },
  // Latin Spanish
  es_lamr: {
    authenticated: {
      userInformation: "Información del usuario",
      accountActions: "Acciones de la cuenta",
      accountOverview: "Cuenta",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "Certificados, ID y perfiles (inglés)",
      viewEvents: "Eventos (inglés)",
      feedbackAssistant: "Feedback Assistant",
      forums: "Foros (inglés)",
      signOut: "Cerrar sesión",
    },
    unauthenticated: {
      signIn: "Iniciar sesión",
    },
  },
  // French
  fr_FR: {
    authenticated: {
      userInformation: "Informations de l'utilisateur",
      accountActions: "Actions du compte",
      accountOverview: "Compte",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "Certificats, identifiants et profils (Anglais)",
      viewEvents: "Évènements (Anglais)",
      feedbackAssistant: "Assistant d'évaluation",
      forums: "Forums (Anglais)",
      signOut: "Se déconnecter",
    },
    unauthenticated: {
      signIn: "Se connecter",
    },
  },
  // Italian
  it_IT: {
    authenticated: {
      userInformation: "Informazioni utente",
      accountActions: "Azioni account",
      accountOverview: "Account",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "Certificati, ID e profili (inglese)",
      viewEvents: "Eventi (inglese)",
      feedbackAssistant: "Feedback Assistant (inglese)",
      forums: "Forum (inglese)",
      signOut: "Esci",
    },
    unauthenticated: {
      signIn: "Accedi",
    },
  },
  // Japanese
  ja_JP: {
    authenticated: {
      userInformation: "ユーザー情報",
      accountActions: "アカウント操作",
      accountOverview: "アカウント",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "証明書、ID、プロファイル（英語）",
      viewEvents: "イベント",
      feedbackAssistant: "フィードバックアシスタント",
      forums: "フォーラム（英語）",
      signOut: "サインアウト",
    },
    unauthenticated: {
      signIn: "サインイン",
    },
  },
  // Korean
  ko_KR: {
    authenticated: {
      userInformation: "사용자 정보",
      accountActions: "계정 관리",
      accountOverview: "계정",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "인증서, ID 및 프로파일(영문)",
      viewEvents: "이벤트",
      feedbackAssistant: "피드백 지원(영문)",
      forums: "포럼(영문)",
      signOut: "로그아웃",
    },
    unauthenticated: {
      signIn: "로그인",
    },
  },
  // Brazilian Portuguese
  pt_BR: {
    authenticated: {
      userInformation: "Informações do usuário",
      accountActions: "Ações da conta",
      accountOverview: "Conta",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "Certificados, IDs e perfis (Inglês)",
      viewEvents: "Eventos (Inglês)",
      feedbackAssistant: "Assistente de Feedback (Inglês)",
      forums: "Fóruns (Inglês)",
      signOut: "Sair",
    },
    unauthenticated: {
      signIn: "Entrar",
    },
  },
  // Simplified Chinese
  zh_CN: {
    authenticated: {
      userInformation: "用户信息",
      accountActions: "账户操作",
      accountOverview: "账户",
      appStoreConnect: "App Store Connect",
      certificatesIDsProfiles: "证书、标识符和描述文件 (英文)",
      viewEvents: "活动",
      feedbackAssistant: "反馈助理 (英文)",
      forums: "论坛 (英文)",
      signOut: "退出登录",
    },
    unauthenticated: {
      signIn: "登录",
    },
  },
};

// Converts a BCP 47 language tag (e.g. "en-US", "ja-JP") to the underscore
// locale format used by the strings dictionary (e.g. "en_US", "ja_JP").
function bcp47ToLocale(tag) {
  return tag.replace(/-/g, "_");
}

// The currently selected dictionary of strings, for a single locale.
let localizedStrings = null;

// Sets the value of `localizedStrings` based on the indicated `locale`.
function loadLocalizedStrings(locale = "en_US") {
  localizedStrings = strings[locale] || strings["en_US"];
}

// Fetches session info and environment metadata from the backend API.
async function fetchSessionInfo() {
  const res = await fetch("/services-session/v1/info");

  if (!res.ok) {
    const error = new Error(`HTTP ${res.status}`);

    error.statusCode = res.status;

    throw error;
  }

  return res.json();
}

// Defines the "AccountMenuPopover" web component.
class AccountMenuPopover extends HTMLElement {
  constructor() {
    super();

    // The trigger element that the popover is visually anchored to,
    // and positioned relative to, when shown.
    this.anchorElement = null;

    // The most recently fetched session payload; set via `setSessionData()`
    // before this component's `render()` method is called.
    this.sessionData = null;
  }

  connectedCallback() {
    // Sets up popover attributes.
    this.setAttribute("popover", "auto");
    this.id = "account-menu-popover";
    this.dataset.aaContentModule = "session-nav";

    // Adds accessibility attributes.
    this.setAttribute("role", "dialog");
    this.setAttribute("aria-label", "Account menu");

    // Sets up positioning.
    this.setupPositioning();
  }

  setAnchor(element) {
    this.anchorElement = element;
  }

  setSessionData(data) {
    this.sessionData = data;
  }

  render() {
    this.renderAuthenticated();
  }

  renderAuthenticated() {
    const template = document.getElementById(
      "account-menu-authenticated-template",
    );

    if (!template) {
      // The component won't render if its corresponding `<template>` isn't
      // found in the DOM.
      return;
    }

    // Clones the template content.
    const content = template.content.cloneNode(true);

    // Retrieves references to individual data "slots" inside of `content`.
    const nameSlot = content.querySelector('[data-slot="name"]');
    const emailSlot = content.querySelector('[data-slot="email"]');
    const initialSlot = content.querySelector('[data-slot="initial"]');

    // Extracts fields from the response data.

    const { attributes } = this.sessionData.data;
    const { firstName, lastName, email, membershipTypes } = attributes;
    const { environment: externalProperties } = this.sessionData.meta;

    const userInitial = firstName ? firstName.charAt(0).toUpperCase() : "?";

    const fullName = `${firstName} ${lastName}`;

    // Injects user details into the data slots.

    if (nameSlot) {
      nameSlot.textContent = fullName;
    }

    if (emailSlot) {
      emailSlot.textContent = email;
      emailSlot.title = email;
    }

    if (initialSlot) {
      initialSlot.textContent = userInitial;
    }

    // Injects localized strings for any resource link labels.

    if (localizedStrings) {
      const slots = [
        "accountOverview",
        "appStoreConnect",
        "certificatesIDsProfiles",
        "viewEvents",
        "feedbackAssistant",
        "forums",
        "signOut",
      ];

      for (const key of slots) {
        const slot = content.querySelector(`[data-slot="${key}"]`);

        if (slot) {
          slot.textContent = localizedStrings.authenticated[key];
        }
      }

      const signOutSlot = content.querySelector('[data-slot="signOut"]');

      if (signOutSlot) {
        const returnPath = encodeURIComponent(window.location.pathname);

        signOutSlot.closest("a").href = `/logout/?return=${returnPath}`;
      }

      // Applies localized strings to aria-label attributes.
      for (const el of content.querySelectorAll("[data-aria-label]")) {
        const key = el.dataset.ariaLabel;
        if (localizedStrings.authenticated[key]) {
          el.setAttribute("aria-label", localizedStrings.authenticated[key]);
        }
      }
    }

    // Injects "external property" URLs for App Store Connect,
    // Feedback Assistant, etc.
    for (const [key, url] of Object.entries(externalProperties)) {
      const slot = content.querySelector(`[data-slot="${key}"]`);

      if (slot) {
        slot.closest("a").href = url;
      }
    }

    // Removes resource links the user is not entitled to based on their
    // Developer Program membership association(s).
    const hasAdpMembership = membershipTypes.includes("ad19");
    const hasPaidMembership = membershipTypes.some((t) => ["ad19", "ee2"].includes(t));

    if (!hasAdpMembership) {
      content.querySelector('[data-slot="appStoreConnect"]')?.closest("a")?.remove();
    }

    if (!hasPaidMembership) {
      content.querySelector('[data-slot="certificatesIDsProfiles"]')?.closest("a")?.remove();
    }

    // Clears the component body and and appends `content`.
    this.innerHTML = "";
    this.appendChild(content);
  }

  setupPositioning() {
    // Positions the popover when it's shown.
    this.addEventListener("toggle", (e) => {
      if (e.newState === "open") {
        this.positionPopover();
      }
    });

    // Dismisses the popover on scroll.
    window.addEventListener("scroll", () => {
      if (this.matches(":popover-open")) {
        this.hidePopover();
      }
    });

    // Repositions the popover on viewport resize.
    window.addEventListener("resize", () => {
      if (this.matches(":popover-open")) {
        this.positionPopover();
      }
    });
  }

  positionPopover() {
    if (!this.anchorElement) {
      // Exits early if no "anchor" element is set.
      return;
    }

    const clientRect = this.anchorElement.getBoundingClientRect();

    const isMobile = window.innerWidth < 768;

    this.style.position = "fixed";
    this.style.top = `${clientRect.bottom + 8}px`;
    this.style.margin = "0";

    if (isMobile) {
      // Centers the popover horizontally on small viewport.
      this.style.left = "50%";
      this.style.right = "auto";
      this.style.transform = "translateX(-50%)";
    } else {
      // Right-aligns the popover on medium and large viewports.
      this.style.right = `${window.innerWidth - clientRect.right}px`;
      this.style.left = "auto";
      this.style.transform = "none";
    }
  }
}

// Registers the custom "AccountMenuPopover" web component.
customElements.define("account-menu-popover", AccountMenuPopover);

// Restores the `href` attribute that the inline script in global-session-nav.inc removed
// before first paint, and clears the associated cursor/data-attribute state.
function restorePendingHref(element) {
  const pendingHref = element.dataset.pendingHref;

  if (pendingHref) {
    // Suppresses `transition: color` inherited from `.globalnav-link` for the
    // duration of the href restore. Adding `href` changes the element's link
    // state (gaining `:link`), which in some browsers causes a color
    // recalculation that triggers the transition and produces a visible flash.
    element.style.transition = "none";
    element.setAttribute("href", pendingHref);
    element.removeAttribute("data-pending-href");
    element.style.cursor = "";
    element.getBoundingClientRect(); // commits the no-transition state
    element.style.transition = "";
  }
}

// Resolves with the first element matching `selector` once it appears in the
// DOM, or `null` if it hasn't appeared within `timeout` milliseconds.
// Resolves immediately if the element is already present.
function waitForElement(selector, timeout = 10_000) {
  return new Promise((resolve) => {
    const element = document.querySelector(selector);

    // Resolves immediately if the element is already in the DOM.
    if (element) {
      resolve(element);

      return;
    }

    // Resolves with `null` once the timeout elapses without a match.
    const timer = setTimeout(() => {
      observer.disconnect();

      resolve(null);
    }, timeout);

    const observer = new MutationObserver(() => {
      const element = document.querySelector(selector);

      // Resolves with the element as soon as it appears in the DOM.
      if (element) {
        clearTimeout(timer);
        observer.disconnect();
        resolve(element);
      }
    });

    observer.observe(document.body, { childList: true, subtree: true });
  });
}

// Initializes the user session menu.
async function initializeAccountMenu() {
  // Waits for the designated "Account" link (trigger) to appear in the DOM.
  // Resolves immediately on SSR pages where the header is present before
  // script execution; waits up to 10 seconds for CSR apps that render the
  // global header at runtime.
  const globalHeaderAccountLink = await waitForElement(".globalnav-link-account");

  // Exits if the trigger element never appeared.
  if (!globalHeaderAccountLink) {
    return;
  }

  // Requires Popover API support. If unavailable, restores the avatar link's
  // `href` (which the inline script in `global-session-nav.inc` removed before first paint)
  // so that the link continues to navigate to `/account`.
  if (typeof HTMLElement.prototype.showPopover !== "function") {
    restorePendingHref(globalHeaderAccountLink);

    return;
  }

  const htmlLang = document.documentElement.lang;

  let locale = htmlLang ? bcp47ToLocale(htmlLang) : "en_US";

  // Loads localized strings for the requested `locale`.
  loadLocalizedStrings(locale);

  // Creates and attaches the popover element ahead of the session fetch.
  // Removed from the DOM if the session fetch indicates the user is not authenticated.

  const popover = document.createElement("account-menu-popover");

  document.body.appendChild(popover);

  popover.setAnchor(globalHeaderAccountLink);

  // Note: we intentionally do not set the native `popovertarget` HTML attribute
  // on the link element. Setting it would cause the browser's native invoker
  // behavior to toggle the popover in addition to our manual click handler,
  // below, resulting in a double-toggle.

  // Captures the popover's open state on `pointerdown`, before `popover="auto"`
  // light-dismiss has a chance to close it. The click handler then uses this
  // to decide whether to open the popover — if it was already open, light-
  // dismiss has already handled the close and no further action is needed.
  let popoverWasOpen = false;

  globalHeaderAccountLink.addEventListener("pointerdown", () => {
    popoverWasOpen = popover.matches(":popover-open");
  });

  globalHeaderAccountLink.addEventListener("click", (e) => {
    if (globalHeaderAccountLink.hasAttribute("disabled")) {
      // Ignores pointer events while session info is pending.
      e.preventDefault();
      return;
    }

    if (popover.isConnected) {
      // Prevents navigation and toggles the popover while the popover is active.
      e.preventDefault();
      if (!popoverWasOpen) {
        popover.showPopover();
      }
    }
    // If the popover was removed (e.g. 404 or 5xx restore path), allows default
    // navigation so the avatar link navigates to /account as normal.
  });

  // Restores the `href` now that the click listener is registered. The listener
  // prevents default navigation, but the `href` is needed for semantics/a11y.
  restorePendingHref(globalHeaderAccountLink);

  globalHeaderAccountLink.setAttribute("disabled", "");

  // Observes html[lang] changes and re-localizes the popover or "Sign In"
  // button immediately.
  let signInLink = null;

  const observer = new MutationObserver(() => {
    const newLocale = bcp47ToLocale(document.documentElement.lang || "en_US");

    if (newLocale === locale) {
      return;
    }

    locale = newLocale;

    loadLocalizedStrings(locale);

    if (signInLink) {
      signInLink.textContent = localizedStrings?.unauthenticated.signIn ?? "Sign In";
    } else {
      popover.render();
    }
  });

  observer.observe(document.documentElement, {
    attributes: true,
    attributeFilter: ["lang"],
  });

  // Fetches session info, then switches the popover out of skeleton state.
  let sessionData = null;
  let fetchError = null;

  try {
    sessionData = await fetchSessionInfo();
  } catch (error) {
    fetchError = error;
  }

  if (!fetchError) {
    // Adds the `.authenticated` class to the account link container,
    // and swaps in the filled avatar SVG.
    globalHeaderAccountLink.closest(".globalnav-account")?.classList.add("authenticated");

    const authenticatedAvatarSVG = `<svg width="18" height="18" viewBox="0 0 18 18" xmlns="http://www.w3.org/2000/svg" focusable="false">
      <g stroke="none" fill="none" fill-rule="evenodd" stroke-width="1">
        <g transform="translate(1.1075, 1.5032)" fill="currentColor" fill-rule="nonzero">
          <path d="M7.22419758,14.4483952 C11.2104441,14.4483952 14.4483952,11.2104441 14.4483952,7.22419758 C14.4483952,3.2379511 11.2104441,0 7.22419758,0 C3.2379511,0 0,3.2379511 0,7.22419758 C0,11.2104441 3.2379511,14.4483952 7.22419758,14.4483952 Z M7.22419758,8.37111762 C5.85348826,8.36412421 4.79042179,7.21720417 4.79042179,5.70662655 C4.78349578,4.27996991 5.86048168,3.09108938 7.22419758,3.09108938 C8.58092005,3.09108938 9.65091251,4.27996991 9.65091251,5.70662655 C9.65091251,7.21720417 8.58791352,8.38510445 7.22419758,8.37111762 Z M7.22419758,13.266508 C5.65767264,13.266508 3.9932399,12.6161205 2.93723425,11.4761938 C3.73448355,10.2733265 5.33597556,9.58097837 7.22419758,9.58097837 C9.09143933,9.58097837 10.6999247,10.2593396 11.5041675,11.4761938 C10.4481619,12.6161205 8.79072255,13.266508 7.22419758,13.266508 Z"></path>
        </g>
      </g>
    </svg>`;

    const parser = new DOMParser();

    const filledInSVG = parser.parseFromString(authenticatedAvatarSVG, "image/svg+xml").documentElement;

    globalHeaderAccountLink.querySelectorAll(".globalnav-link-image").forEach((span) => {
      const defaultAvatarSVG = span.querySelector("svg");

      if (defaultAvatarSVG) {
        defaultAvatarSVG.replaceWith(filledInSVG.cloneNode(true));
      }
    });

    // Re-enables interactivity for the account link (trigger).
    globalHeaderAccountLink.removeAttribute("disabled");
    globalHeaderAccountLink.style.cursor = "";

    // Populates the popover element's data "slots" with session data.
    popover.setSessionData(sessionData);

    // Renders the authenticated popover menu with personalized resource links.
    popover.render();
  } else if (
    fetchError !== null &&
    fetchError.statusCode !== 401
  ) {
    // A non-authentication error was encountered. This includes 404 (user is
    // authenticated but has not yet accepted the RAD agreement) as well as 5xx
    // and network failures (authentication state unknown). In all cases, restore
    // the avatar link so the user can still navigate to `/account` directly;
    // remove the un-showable popover.
    globalHeaderAccountLink.removeAttribute("disabled");
    globalHeaderAccountLink.style.cursor = "";

    popover.remove();
  } else {
    // A 401 was encountered, meaning the user is definitively not signed in.
    // Replaces the avatar link with a "Sign In" button that navigates to the
    // login page, passing the current path as a `return` parameter so the user
    // is redirected back after signing in.
    const returnPath = encodeURIComponent(window.location.pathname);

    signInLink = document.createElement("a");
    signInLink.className = "button button-reduced";
    signInLink.href = `/login/?return=${returnPath}`;
    signInLink.textContent = localizedStrings?.unauthenticated.signIn ?? "Sign In";

    globalHeaderAccountLink.closest(".globalnav-account")?.classList.add("unauthenticated");
    globalHeaderAccountLink.replaceWith(signInLink);

    popover.remove();
  }
}

// Initializes when DOM is ready
initializeAccountMenu();
