((window, undefined) => {
	function onDidChangeSystemColorScheme() {
		if (window.__disableColorScheme) return; // Block updates if theme is locked

		AppStore.state.systemColorScheme = prefersColorSchemeMediaQueryList.matches
			? ColorScheme.dark.value
			: ColorScheme.light.value;

		if (AppStore.state.preferredColorScheme === ColorScheme.auto.value) {
			setPreferredColorScheme(ColorScheme.auto.value);
		}
	}

	function setPreferredColorScheme(colorSchemeValue) {
		if (window.__disableColorScheme) return; // Prevent changes if locked

		AppStore.setPreferredColorScheme(colorSchemeValue);

		const optionInputs = Array.from(
			window.document.querySelectorAll(
				'label[data-color-scheme-option] > input[type="radio"]',
			),
		);

		Array.from(optionInputs).map((option) => {
			option.checked = !!(option.value === colorSchemeValue);
		});

		const prefersDark = !!(
			colorSchemeValue === ColorScheme.dark.value ||
			(colorSchemeValue === ColorScheme.auto.value &&
				AppStore.state.systemColorScheme === ColorScheme.dark.value)
		);

		if (prefersDark) {
			ColorSchemeDarkHead.map((node) => {
				node.parentElement === null && document.head.appendChild(node);
			});

			document.body.dataset.colorScheme = ColorScheme.dark.value;
			setBodyColorThemeClass("dark");
		} else {
			ColorSchemeDarkHead.map((node) => {
				node.parentElement && node.parentElement.removeChild(node);
			});

			document.body.dataset.colorScheme = ColorScheme.light.value;
			setBodyColorThemeClass("light");
		}
	}

	function setBodyColorThemeClass(theme) {
		if (window.__disableColorScheme) return; // Block class updates if locked

		if (theme === "dark") {
			document.body.classList.add("theme-dark");
			document.body.classList.remove("theme-light");
		} else if (theme === "light") {
			document.body.classList.add("theme-light");
			document.body.classList.remove("theme-dark");
		}
	}

	const ColorScheme = {
		auto: { value: "auto" },
		light: { value: "light" },
		dark: { value: "dark" },
	};

	const ColorSchemeLocales = {
		en: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Auto",
			light: "Light",
			dark: "Dark",
		},
		zh_CN: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "自动",
			light: "浅色",
			dark: "深色",
		},
		ja_JP: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "自動",
			light: "ライト",
			dark: "ダーク",
		},
		ko_KR: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "자동",
			light: "라이트",
			dark: "다크",
		},
		it_IT: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatico",
			light: "Chiaro",
			dark: "Scuro",
		},
		fr_FR: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatique",
			light: "Clair",
			dark: "Sombre",
		},
		de_DE: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatisch",
			light: "Hell",
			dark: "Dunkel",
		},
		pt_BR: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automática",
			light: "Clara",
			dark: "Escura",
		},
		es_lamr: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automático",
			light: "Claro",
			dark: "Obscuro",
		},
		es_LA: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automático",
			light: "Claro",
			dark: "Obscuro",
		},
		es_419: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automático",
			light: "Claro",
			dark: "Obscuro",
		},
		zh_TW: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "自動",
			light: "淺色",
			dark: "深色",
		},
		bg_BG: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Автоматично",
			light: "Светла",
			dark: "Тъмна",
		},
		cs_CZ: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automaticky",
			light: "Světlý",
			dark: "Tmavý",
		},
		da_DK: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Auto",
			light: "Lys",
			dark: "Mørk",
		},
		et_EE: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automaatne",
			light: "Hele",
			dark: "Tume",
		},
		es_ES: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automático",
			light: "Claro",
			dark: "Oscuro",
		},
		fi_FI: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automaattinen",
			light: "Vaalea",
			dark: "Tumma",
		},
		el_GR: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Αυτόματο",
			light: "Ανοιχτόχρωμο",
			dark: "Σκουρόχρωμο",
		},
		hr_HR: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatski",
			light: "Svijetla",
			dark: "Tamna",
		},
		hu_HU: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatikus",
			light: "Világos",
			dark: "Sötét",
		},
		ga_IE: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Auto",
			light: "Light",
			dark: "Dark",
		},
		lt_LT: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatinis",
			light: "Šviesus",
			dark: "Tamsus",
		},
		lv_LV: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automātiski",
			light: "Gaišs",
			dark: "Tumšs",
		},
		mt_MT: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Awtomatiku",
			light: "Ċar",
			dark: "Skur",
		},
		nl_NL: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatisch",
			light: "Licht",
			dark: "Donker",
		},
		pl_PL: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatyczny",
			light: "Jasny",
			dark: "Ciemny",
		},
		pt_PT: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automático",
			light: "Claro",
			dark: "Escuro",
		},
		ro_RO: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automat",
			light: "Luminos",
			dark: "Întunecat",
		},
		sv_SE: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automatiskt",
			light: "Ljust",
			dark: "Mörkt",
		},
		sl_SI: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Samodejno",
			light: "Svetlo",
			dark: "Temno",
		},
		sk_SK: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Automaticky",
			light: "Svetlá",
			dark: "Tmavá",
		},
		en_GB: {
			toggleAriaLabel: "Select a color scheme preference",
			auto: "Auto",
			light: "Light",
			dark: "Dark",
		},
	};

	const ColorSchemeToggleRadioGroup = document.querySelector(
		".color-scheme-toggle",
	);

	const ColorSchemeToggleRadioLabels =
		ColorSchemeToggleRadioGroup?.querySelectorAll(".text");

	const ColorSchemeDarkHead = Array.from(
		document.head.querySelectorAll('[data-color-scheme="dark"]'),
	);

	let localeKey = "en";

	const htmlLangNode = ColorSchemeToggleRadioGroup?.closest("[lang]");

	if (htmlLangNode) {
		localeKey = htmlLangNode.lang
			.replace("en_US", "en")
			.replace("en-US", "en")
			.replace("-", "_");
	}

	const Locale = ColorSchemeLocales[localeKey] || ColorSchemeLocales.en;

	ColorSchemeDarkHead.map((node) => (node.media = ""));

	ColorSchemeToggleRadioGroup?.setAttribute(
		"aria-label",
		Locale.toggleAriaLabel,
	);

	Array.from(ColorSchemeToggleRadioLabels || []).map((label) => {
		label.textContent = Locale[label.parentElement.dataset.colorSchemeOption];
	});

	const supportsAutoColorScheme =
		typeof window.matchMedia !== "undefined" &&
		[ColorScheme.light.value, ColorScheme.dark.value, "no-preference"].some(
			(scheme) =>
				window.matchMedia(`(prefers-color-scheme: ${scheme})`).matches,
		);

	const defaultColorScheme = supportsAutoColorScheme
		? ColorScheme.auto
		: ColorScheme.light;

	window.Settings = window.Settings || {};

	const AppStore = {
		state: {
			preferredColorScheme:
				window.Settings.preferredColorScheme || defaultColorScheme.value,
			supportsAutoColorScheme,
			systemColorScheme: ColorScheme.light.value,
		},
		setPreferredColorScheme(value) {
			if (window.__disableColorScheme) return; // Block preference updates if locked
			this.state.preferredColorScheme = value;
			window.Settings.preferredColorScheme = value;
		},
		setSystemColorScheme(value) {
			this.state.systemColorScheme = value;
		},
		syncPreferredColorScheme() {
			if (window.__disableColorScheme) return; // Prevent sync if locked
			if (
				!!Settings.preferredColorScheme &&
				Settings.preferredColorScheme !== this.state.preferredColorScheme
			) {
				this.state.preferredColorScheme = Settings.preferredColorScheme;
			}
		},
	};

	const prefersColorSchemeMediaQueryList = window.matchMedia(
		"(prefers-color-scheme: dark)",
	);

	if (supportsAutoColorScheme) {
		AppStore.state.systemColorScheme = prefersColorSchemeMediaQueryList.matches
			? ColorScheme.dark.value
			: ColorScheme.light.value;
	}

	try {
		prefersColorSchemeMediaQueryList.addEventListener(
			"change",
			onDidChangeSystemColorScheme,
		);
	} catch (e) {
		prefersColorSchemeMediaQueryList.addListener(onDidChangeSystemColorScheme);
	}

	if (supportsAutoColorScheme === false) {
		window.document.body.setAttribute("data-supports-auto-color-scheme", false);
	}

	// Only set the preferred color scheme if it's NOT disabled
	if (!window.__disableColorScheme) {
		setPreferredColorScheme(AppStore.state.preferredColorScheme);
	}

	window.ColorScheme = ColorScheme;

	window.setPreferredColorScheme = setPreferredColorScheme;

	window.addEventListener("pageshow", () => {
		if (!window.__disableColorScheme) {
			AppStore.syncPreferredColorScheme();
		}
	});

	function colorSchemeHotKeys(e) {
		if (window.__disableColorScheme) return; // Disable shortcuts if locked

		if (e.ctrlKey && e.which == 73) {
			const invertColorSchemeValue =
				document.body.dataset.colorScheme === "dark" ? "light" : "dark";
			setPreferredColorScheme(invertColorSchemeValue);
		}

		if (e.ctrlKey && e.shiftKey && e.which == 73) {
			setPreferredColorScheme(ColorScheme.auto.value);
		}
	}

	// Only enable hotkeys if color scheme is not locked
	if (!window.__disableColorScheme) {
		document.addEventListener("keyup", colorSchemeHotKeys, false);
	}
})(window);
