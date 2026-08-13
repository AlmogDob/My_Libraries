window.addEventListener('DOMContentLoaded', () => {
	// Define the language strings

	var langs = {
		"en": "English",
		"zh-CN": "简体中文",
		"zh-TW": "繁體中文",
		"ja-JP": "日本語",
		"ko-KR": "한국어",
		"de-DE": "Deutsch",
		"fr-FR": "Français",
		"it-IT": "Italiano",
		"pt-BR": "Português (Brasil)",
		"bg-BG": "Български",
		"el-GR": "Ελληνικά",
		"cs-CZ": "Čeština",
		"da-DK": "Dansk",
		"et-EE": "Eesti",
		"en-GB": "English (UK)",
		"es-ES": "Español (España)",
		"es-419": "Español (Latinoamérica)",
		"ga-IE": "Gaeilge",
		"hr-HR": "Hrvatski",
		"lv-LV": "Latviešu",
		"lt-LT": "Lietuvių",
		"ro-RO": "Română",
		"hu-HU": "Magyar",
		"mt-MT": "Malti",
		"nl-NL": "Nederlands",
		"pl-PL": "Polski",
		"pt-PT": "Português (Portugal)",
		"sk-SK": "Slovenčina",
		"sl-SI": "Slovenščina",
		"fi-FI": "Suomi",
		"sv-SE": "Svenska"
	}

	// Define the default language display order

	var order = [
		'en',
		'zh-CN',
		'zh-TW',
		'ja-JP',
		'ko-KR',
		'bg-BG',
		'el-GR',
		'cs-CZ',
		'da-DK',
		'de-DE',
		'et-EE',
		'en-GB',
		'es-ES',
		'es-419',
		'fr-FR',
		'ga-IE',
		'hr-HR',
		'it-IT',
		'lv-LV',
		'lt-LT',
		'ro-RO',
		'hu-HU',
		'mt-MT',
		'nl-NL',
		'pl-PL',
		'pt-PT',
		'pt-BR',
		'sk-SK',
		'sl-SI',
		'fi-FI',
		'sv-SE',
	];

	// Cache values from the page

	var pagelang = document.querySelector('html').getAttribute('navlang');
	if(pagelang === null) {
		var pagelang = document.querySelector('html').getAttribute('lang');
	}

	var alternates = document.querySelectorAll('link[rel="alternate"][hreflang]');
	var dropdown = document.querySelector('.language-dropdown');
	var menu = document.querySelector('.language-dropdown select');

	// If required data exists on page...

	if(order.includes(pagelang) && alternates.length > 1 && dropdown.getAttribute('data-lang') !== 'complete') {

		// Bubble up the current language

		if(order.includes(pagelang)) {
			order.splice(order.indexOf(pagelang), 1);
			order.unshift(pagelang);
		}

		// Loop through languages in order

		Object.entries(order).forEach(([index, lang]) => {

			// Locate language tag

			alternate = document.querySelector('link[rel="alternate"][hreflang="' + lang + '"]');

			// If language exists on page...

			if(alternate !== null) {

				// Create a new item for language

				var node = document.createElement('option');
				var href = alternate.getAttribute('href');
				var text = document.createTextNode(langs[lang]);

				node.appendChild(text);
				node.value = href;
				menu.appendChild(node);

				// If language matches current language...

				if(lang == pagelang) {

					// Set the menu to the current language

					menu.value = href;

				}
			}
		});

		// Add redirect based on language selection

		menu.addEventListener('change', function() {
			window.location = this.value;
		})

		// Swap visible language selector

		dropdown.classList.remove('hidden');
		dropdown.setAttribute('data-lang', 'complete');
	}
});