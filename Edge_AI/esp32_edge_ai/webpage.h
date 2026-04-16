#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>AquaAnalyzer - Dashboard</title>
    <style>
        /* Reset e Base */
        :root {
            /* Tema Claro (Padrão) */
            --bg-primary: #f8fafc;
            --bg-secondary: #ffffff;
            --bg-tertiary: #f3f4f6;
            --text-primary: #1f2937;
            --text-secondary: #4b5563;
            --text-tertiary: #6b7280;
            --border-color: #e2e8f0;
            --accent-color: #2563eb;
            --accent-gradient: linear-gradient(135deg, #60a5fa 0%, #2563eb 100%);
            --success-bg: #d1fae5;
            --success-text: #065f46;
            --success-border: #a7f3d0;
            --warning-bg: #fef3c7;
            --warning-text: #92400e;
            --warning-border: #fcd34d;
            --danger-bg: #fee2e2;
            --danger-text: #b91c1c;
            --danger-border: #f87171;
            --shadow-sm: 0 1px 2px rgba(0, 0, 0, 0.05);
            --shadow-md: 0 4px 6px rgba(0, 0, 0, 0.1);
            --nav-active: #10b981;
        }

        [data-theme="dark"] {
            --bg-primary: #0f172a;
            --bg-secondary: #1e293b;
            --bg-tertiary: #334155;
            --text-primary: #f1f5f9;
            --text-secondary: #94a3b8;
            --text-tertiary: #64748b;
            --border-color: #334155;
            --accent-color: #3b82f6;
            --accent-gradient: linear-gradient(135deg, #3b82f6 0%, #1d4ed8 100%);
            --success-bg: #064e3b;
            --success-text: #34d399;
            --success-border: #059669;
            --warning-bg: #78350f;
            --warning-text: #fcd34d;
            --warning-border: #d97706;
            --danger-bg: #7f1d1d;
            --danger-text: #fca5a5;
            --danger-border: #dc2626;
            --shadow-sm: 0 1px 2px rgba(0, 0, 0, 0.3);
            --shadow-md: 0 4px 6px rgba(0, 0, 0, 0.4);
            --nav-active: #34d399;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        html {
            scroll-behavior: smooth;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
            background-color: var(--bg-primary);
            color: var(--text-primary);
            line-height: 1.6;
            transition: background-color 0.3s ease, color 0.3s ease;
        }

        /* Cabeçalho */
        .header {
            background-color: var(--bg-secondary);
            border-bottom: 1px solid var(--border-color);
            position: sticky;
            top: 0;
            z-index: 50;
            box-shadow: var(--shadow-sm);
            transition: background-color 0.3s ease, border-color 0.3s ease;
        }

        .header-container {
            max-width: 1280px;
            margin: 0 auto;
            padding: 1rem;
            display: flex;
            align-items: center;
            justify-content: space-between;
        }

        .logo {
            display: flex;
            align-items: center;
            gap: 0.5rem;
            text-decoration: none;
            color: var(--text-primary);
        }

        .logo-icon {
            width: 2rem;
            height: 2rem;
            background: var(--accent-gradient);
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            color: white;
        }

        .logo-icon svg {
            width: 1.25rem;
            height: 1.25rem;
            stroke-width: 2;
        }

        .logo-text {
            font-size: 1.25rem;
            font-weight: 700;
            color: var(--text-primary);
        }

        /* Navegação */
        .nav {
            display: flex;
            align-items: center;
            gap: 1.5rem;
        }

        .nav-item {
            display: flex;
            align-items: center;
            gap: 0.5rem;
            background: none;
            border: 1px solid var(--border-color);
            border-radius: 0.375rem;
            color: var(--text-secondary);
            cursor: pointer;
            padding: 0.5rem 1rem;
            font-size: 0.875rem;
            font-weight: 500;
            transition: color 0.2s ease;
            transition: all 0.2s ease;
        }

        .nav-item:hover {
            color: var(--text-primary);
            border-color: var(--accent-color);
            background-color: var(--bg-tertiary);
        }

        .nav-item:first-child {
            color: var(--nav-active);
            border-color: var(--nav-active);
        }

        .icon-small {
            width: 1rem;
            height: 1rem;
            stroke-width: 2;
        }

        /* Conteúdo Principal */
        .main-content {
            max-width: 1280px;
            margin: 0 auto;
            padding: 2rem 1rem;
        }

        .container {
            width: 100%;
        }

        /* Seção da Última Análise */
        .last-analysis {
            background-color: var(--bg-secondary);
            border-radius: 0.5rem;
            border: 1px solid var(--border-color);
            padding: 1.5rem;
            margin-bottom: 2rem;
            display: flex;
            align-items: center;
            justify-content: space-between;
            box-shadow: var(--shadow-sm);
            transition: background-color 0.3s ease, border-color 0.3s ease;
        }

        .analysis-title {
            font-size: 1.25rem;
            font-weight: 600;
            color: var(--text-primary);
            margin-bottom: 0.25rem;
        }

        .analysis-time {
            font-size: 0.875rem;
            color: var(--text-tertiary);
        }

        .quality-badge {
            display: inline-flex;
            align-items: center;
            padding: 0.5rem 1rem;
            background-color: var(--success-bg);
            color: var(--success-text);
            border-radius: 9999px;
            font-size: 0.875rem;
            font-weight: 500;
            border: 1px solid var(--success-border);
        }



        /* Grade de Métricas */
        .metrics-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 1.5rem;
            margin-bottom: 2rem;
        }

        .metric-card {
            background-color: var(--bg-secondary);
            border-radius: 0.5rem;
            border: 1px solid var(--border-color);
            padding: 1.5rem;
            box-shadow: var(--shadow-sm);
            transition: box-shadow 0.2s ease, background-color 0.3s ease, border-color 0.3s ease;
        }

        .metric-card:hover {
            box-shadow: var(--shadow-md);
        }

        .metric-header {
            display: flex;
            align-items: flex-start;
            justify-content: space-between;
            margin-bottom: 1rem;
        }

        .metric-title {
            font-size: 0.875rem;
            font-weight: 600;
            color: var(--text-primary);
        }

        .metric-icon {
            width: 1.25rem;
            height: 1.25rem;
            color: var(--text-secondary);
            stroke-width: 2;
        }

        .metric-value {
            font-size: 1.875rem;
            font-weight: 700;
            color: var(--text-primary);
            margin-bottom: 0.5rem;
        }

        .metric-description {
            font-size: 0.75rem;
            color: var(--text-tertiary);
            margin-bottom: 1rem;
        }

        .metric-footer {
            display: flex;
            justify-content: flex-end;
        }

        .status-badge {
            display: inline-flex;
            align-items: center;
            padding: 0.375rem 0.75rem;
            border-radius: 9999px;
            font-size: 0.75rem;
            font-weight: 500;
            border: 1px solid;
            transition: all 0.3s ease;
        }
        
        /* Status Colors */
        .status-good {
            background-color: var(--success-bg);
            color: var(--success-text);
            border-color: var(--success-border);
        }
        
        .status-medium {
            background-color: var(--warning-bg);
            color: var(--warning-text);
            border-color: var(--warning-border);
        }
        
        .status-bad {
            background-color: var(--danger-bg);
            color: var(--danger-text);
            border-color: var(--danger-border);
        }

        /* Modal/Sobreposição de Página */
        .page-overlay {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: rgba(0, 0, 0, 0.5);
            display: none;
            z-index: 40;
        }

        .page-overlay.active {
            display: block;
        }

        /* Estilos da Página de Configuração */
        .config-page {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: var(--bg-primary);
            display: none;
            z-index: 50;
            overflow-y: auto;
            transition: background-color 0.3s ease;
        }

        .config-page.active {
            display: flex;
            flex-direction: column;
            animation: slideIn 0.3s ease;
        }

        .config-header {
            background-color: var(--bg-secondary);
            border-bottom: 1px solid var(--border-color);
            padding: 1rem;
            display: flex;
            align-items: center;
            justify-content: space-between;
            box-shadow: var(--shadow-sm);
            transition: background-color 0.3s ease, border-color 0.3s ease;
        }

        .config-header-left {
            display: flex;
            align-items: center;
            gap: 1rem;
        }

        .back-btn {
            background: none;
            border: 1px solid var(--border-color);
            border-radius: 0.375rem;
            color: var(--text-secondary);
            cursor: pointer;
            font-size: 0.875rem;
            font-weight: 500;
            display: flex;
            align-items: center;
            gap: 0.5rem;
            transition: color 0.2s ease;
            padding: 0.5rem;
        }

        .back-btn:hover {
            color: var(--text-primary);
            border-color: var(--accent-color);
            background-color: var(--bg-tertiary);
        }

        .config-title {
            font-size: 1.25rem;
            font-weight: 600;
            color: var(--text-primary);
        }

        .save-btn {
            background-color: var(--text-primary);
            color: var(--bg-secondary);
            border: 1px solid var(--text-primary);
            padding: 0.5rem 1rem;
            border-radius: 0.375rem;
            font-size: 0.875rem;
            font-weight: 500;
            cursor: pointer;
            display: flex;
            align-items: center;
            gap: 0.5rem;
            transition: background-color 0.2s ease;
        }

        .save-btn:hover {
            opacity: 0.9;
            box-shadow: 0 0 0 2px var(--bg-secondary), 0 0 0 4px var(--text-primary);
        }

        .config-content {
            flex: 1;
            max-width: 1280px;
            margin: 0 auto;
            width: 100%;
            padding: 2rem 1rem;
        }

        .config-section {
            background-color: var(--bg-secondary);
            border-radius: 0.5rem;
            border: 1px solid var(--border-color);
            padding: 1.5rem;
            margin-bottom: 1.5rem;
            box-shadow: var(--shadow-sm);
            transition: background-color 0.3s ease, border-color 0.3s ease;
        }

        .config-section-header {
            display: flex;
            align-items: flex-start;
            gap: 1rem;
            margin-bottom: 1.5rem;
        }

        .config-section-icon {
            width: 1.5rem;
            height: 1.5rem;
            color: var(--text-secondary);
            stroke-width: 2;
            flex-shrink: 0;
        }

        .config-section-text h3 {
            font-size: 1rem;
            font-weight: 600;
            color: var(--text-primary);
            margin-bottom: 0.25rem;
        }

        .config-section-text p {
            font-size: 0.875rem;
            color: var(--text-tertiary);
        }

        /* Emblemas de Status */
        .status-container {
            display: flex;
            gap: 1rem;
            flex-wrap: wrap;
            margin-bottom: 1.5rem;
        }

        .status-item {
            display: flex;
            align-items: center;
            gap: 0.5rem;
        }

        .status-item-icon {
            width: 1.25rem;
            height: 1.25rem;
            color: var(--text-secondary);
            stroke-width: 2;
        }

        .status-item-name {
            font-size: 0.875rem;
            font-weight: 500;
            color: var(--text-primary);
        }

        .status-item-badge {
            display: inline-flex;
            align-items: center;
            padding: 0.375rem 0.75rem;
            border-radius: 9999px;
            font-size: 0.75rem;
            font-weight: 500;
            border: 1px solid;
        }

        .status-item-badge.connected {
            background-color: var(--success-bg);
            color: var(--success-text);
            border-color: var(--success-border);
        }

        .sync-btn {
            background-color: var(--bg-secondary);
            border: 1px solid var(--border-color);
            padding: 0.5rem 1rem;
            border-radius: 0.375rem;
            font-size: 0.875rem;
            font-weight: 500;
            cursor: pointer;
            display: flex;
            align-items: center;
            gap: 0.5rem;
            color: var(--text-primary);
            transition: background-color 0.2s ease;
        }

        .sync-btn:hover {
            background-color: var(--bg-tertiary);
        }

        /* Itens de Configuração */
        .config-item {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 1rem 0;
            border-bottom: 1px solid var(--bg-tertiary);
        }

        .config-item:last-child {
            border-bottom: none;
        }

        .config-item-left {
            display: flex;
            align-items: center;
            gap: 0.75rem;
        }

        .config-item-icon {
            width: 1.25rem;
            height: 1.25rem;
            color: var(--text-secondary);
            stroke-width: 2;
        }

        .config-item-text h4 {
            font-size: 0.875rem;
            font-weight: 600;
            color: var(--text-primary);
            margin-bottom: 0.25rem;
        }

        .config-item-text p {
            font-size: 0.75rem;
            color: var(--text-tertiary);
        }

        /* Botão de Alternância */
        .toggle-switch {
            position: relative;
            width: 3rem;
            height: 1.5rem;
            background-color: var(--bg-tertiary);
            border-radius: 9999px;
            cursor: pointer;
            border: none;
            padding: 0;
            transition: background-color 0.2s ease;
        }

        .toggle-switch.active {
            background-color: var(--text-primary);
        }

        .toggle-switch::after {
            content: '';
            position: absolute;
            top: 0.25rem;
            left: 0.25rem;
            width: 1rem;
            height: 1rem;
            background-color: var(--bg-secondary);
            border-radius: 50%;
            transition: left 0.2s ease;
        }

        .toggle-switch.active::after {
            left: 1.75rem;
        }

        /* Campos de Entrada */
        .config-input {
            width: 100%;
            padding: 0.75rem;
            border: 1px solid var(--border-color);
            border-radius: 0.375rem;
            font-size: 0.875rem;
            color: var(--text-primary);
            background-color: var(--bg-tertiary);
            margin-top: 0.5rem;
            font-family: inherit;
        }

        .config-input:focus {
            outline: none;
            border-color: var(--accent-color);
            background-color: var(--bg-secondary);
        }

        .config-input:disabled {
            background-color: var(--bg-tertiary);
            color: var(--text-secondary);
            cursor: not-allowed;
        }

        /* Menu Suspenso de Seleção */
        .config-select {
            width: 100%;
            padding: 0.75rem;
            border: 1px solid var(--border-color);
            border-radius: 0.375rem;
            font-size: 0.875rem;
            color: var(--text-primary);
            background-color: var(--bg-tertiary);
            margin-top: 0.5rem;
            cursor: pointer;
            font-family: inherit;
            appearance: none;
            background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='12' viewBox='0 0 12 12'%3E%3Cpath fill='%234b5563' d='M6 9L1 4h10z'/%3E%3C/svg%3E");
            background-repeat: no-repeat;
            background-position: right 0.75rem center;
            padding-right: 2rem;
        }

        [data-theme="dark"] .config-select {
            background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='12' viewBox='0 0 12 12'%3E%3Cpath fill='%239ca3af' d='M6 9L1 4h10z'/%3E%3C/svg%3E");
        }

        .config-select:focus {
            outline: none;
            border-color: var(--accent-color);
            background-color: var(--bg-secondary);
        }

        .config-field {
            margin-bottom: 1.5rem;
        }

        .config-field-label {
            display: block;
            font-size: 0.875rem;
            font-weight: 600;
            color: var(--text-primary);
            margin-bottom: 0.5rem;
        }

        /* Animações */
        @keyframes slideIn {
            from {
                transform: translateX(100%);
                opacity: 0;
            }

            to {
                transform: translateX(0);
                opacity: 1;
            }
        }
    </style>
    <style>
        /* Responsividade para Mobile (Tablets e Celulares) */
        @media (max-width: 768px) {
            .header-container {
                padding: 0.75rem;
            }

            .logo-text {
                font-size: 1.1rem;
            }

            .nav {
                gap: 0.75rem;
            }

            .nav-item {
                padding: 0.5rem;
            }

            .nav-item span {
                display: none;
            }

            .main-content {
                padding: 1.5rem 0.75rem;
            }

            .last-analysis {
                flex-direction: column;
                align-items: flex-start;
                gap: 0.5rem;
            }
            
            .metric-value {
                font-size: 1.5rem;
            }
            
            .config-header {
                padding: 0.75rem;
            }

            .config-title {
                font-size: 1.1rem;
                margin-left: 0.5rem;
            }
            
            .back-btn span, .save-btn span {
                display: none;
            }
        }
    </style>
</head>

<body>
    <!-- Cabeçalho -->
    <header class="header">
        <div class="header-container">
            <span class="logo-text">Hydrosense</span>
            <nav class="nav">
                <button class="nav-item">
                    <svg class="icon-small" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                        <circle cx="12" cy="12" r="1"></circle>
                        <path
                            d="M12 2v2M12 20v2M4.22 4.22l1.41 1.41M17.37 17.37l1.41 1.41M2 12h2M20 12h2M4.22 19.78l1.41-1.41M17.37 6.63l1.41-1.41">
                        </path>
                    </svg>
                    <span>Conectado</span>
                </button>

                <button class="nav-item" id="configBtn">
                    <svg class="icon-small" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <circle cx="12" cy="12" r="3"></circle>
                        <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path>
                    </svg>
                    <span>Config</span>
                </button>

                <button class="nav-item" id="aboutBtn">
                    <svg class="icon-small" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <circle cx="12" cy="12" r="10"></circle>
                        <path d="M9.09 9a3 3 0 0 1 5.83 1c0 2-3 3-3 3"></path>
                        <line x1="12" y1="17" x2="12.01" y2="17"></line>
                    </svg>
                    <span>Sobre</span>
                </button>

                <button class="nav-item" id="themeToggleBtn" title="Alternar Tema">
                    <!-- Icone do Sol (modo escuro) -->
                    <svg class="icon-small sun-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor"
                        stroke-width="2" style="display: none;">
                        <circle cx="12" cy="12" r="5"></circle>
                        <line x1="12" y1="1" x2="12" y2="3"></line>
                        <line x1="12" y1="21" x2="12" y2="23"></line>
                        <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line>
                        <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line>
                        <line x1="1" y1="12" x2="3" y2="12"></line>
                        <line x1="21" y1="12" x2="23" y2="12"></line>
                        <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line>
                        <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line>
                    </svg>
                    <!-- Icon (modo claro) -->
                    <svg class="icon-small moon-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor"
                        stroke-width="2">
                        <path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"></path>
                    </svg>
                </button>

            </nav>
        </div>
    </header>

    <!-- Conteúdo Principal -->
    <main class="main-content">


            <!-- Grade de Métricas -->
            <div class="metrics-grid">
                <!-- Card de pH -->
                <div class="metric-card">
                    <div class="metric-header">
                        <h3 class="metric-title">pH</h3>
                        <svg class="metric-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                            <polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline>
                        </svg>
                    </div>
                    <div class="metric-value" id="phValue">--</div>
                    <p class="metric-description">Potencial hidrogeniônico</p>
                    <div class="metric-footer">
                        <div id="phStatus" class="status-badge">--</div>
                    </div>
                </div>

                <!-- Card de Turbidez -->
                <div class="metric-card">
                    <div class="metric-header">
                        <h3 class="metric-title">Turbidez</h3>
                        <svg class="metric-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                            <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path>
                        </svg>
                    </div>
                    <div class="metric-value" id="turbidezValue">--NTU</div>
                    <p class="metric-description">Claridade da água</p>
                    <div class="metric-footer">
                        <div id="turbidezStatus" class="status-badge">--</div>
                    </div>
                </div>

                <!-- Card de Temperatura -->
                <div class="metric-card">
                    <div class="metric-header">
                        <h3 class="metric-title">Temperatura</h3>
                        <svg class="metric-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                            <path d="M12 2v20M12 2a3 3 0 0 1 3 3v8a3 3 0 0 1-3 3 3 3 0 0 1-3-3V5a3 3 0 0 1 3-3z"></path>
                        </svg>
                    </div>
                    <div class="metric-value" id="tempValue">--°C</div>
                    <p class="metric-description">Temperatura da água</p>
                    <div class="metric-footer">
                        <div id="tempStatus" class="status-badge">--</div>
                    </div>
                </div>
            </div>
        </div>
    </main>

    <script>
        const appState = {
            currentPage: 'dashboard',
            theme: 'light',
            config: {
                temperatureUnit: 'celsius'
            },
            currentTempCelsius: 0
        };

        document.addEventListener('DOMContentLoaded', () => {
            initializeTheme();
            initializeEventListeners();
            loadConfigFromStorage();
            updateDashboard();

            // Fetch Real Sensor Data Loop
            fetchSensorData();
            setInterval(fetchSensorData, 1000);
        });

        function initializeTheme() {
            const savedTheme = localStorage.getItem('aquaanalyzer-theme');
            if (savedTheme) {
                appState.theme = savedTheme;
                document.documentElement.setAttribute('data-theme', savedTheme);
            } else if (window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches) {
                appState.theme = 'dark';
                document.documentElement.setAttribute('data-theme', 'dark');
            }
            updateThemeIcon();
        }

        function toggleTheme() {
            appState.theme = appState.theme === 'light' ? 'dark' : 'light';
            document.documentElement.setAttribute('data-theme', appState.theme);
            localStorage.setItem('aquaanalyzer-theme', appState.theme);
            updateThemeIcon();
        }

        function updateThemeIcon() {
            const sunIcon = document.querySelector('.sun-icon');
            const moonIcon = document.querySelector('.moon-icon');
            if (sunIcon && moonIcon) {
                if (appState.theme === 'dark') {
                    sunIcon.style.display = 'block';
                    moonIcon.style.display = 'none';
                } else {
                    sunIcon.style.display = 'none';
                    moonIcon.style.display = 'block';
                }
            }
        }

        function initializeEventListeners() {
            const configBtn = document.getElementById('configBtn');
            if (configBtn) configBtn.addEventListener('click', showConfigPage);
            const aboutBtn = document.getElementById('aboutBtn');
            if (aboutBtn) aboutBtn.addEventListener('click', showAboutPage);
            const themeToggleBtn = document.getElementById('themeToggleBtn');
            if (themeToggleBtn) themeToggleBtn.addEventListener('click', toggleTheme);
            document.addEventListener('click', (e) => {
                if (e.target.closest('.back-btn')) showDashboard();
                if (e.target.closest('.save-btn')) saveConfig();
            });
        }

        function showConfigPage() {
            if (!document.querySelector('.config-page')) createConfigPage();
            const configPage = document.querySelector('.config-page');
            const overlay = document.querySelector('.page-overlay');
            configPage.classList.add('active');
            if (overlay) overlay.classList.add('active');
            populateConfigForm();
        }

        function showAboutPage() {
            if (!document.querySelector('.about-page')) createAboutPage();
            const aboutPage = document.querySelector('.about-page');
            const overlay = document.querySelector('.page-overlay');
            aboutPage.classList.add('active');
            if (overlay) overlay.classList.add('active');
        }

        function showDashboard() {
            const configPage = document.querySelector('.config-page');
            const aboutPage = document.querySelector('.about-page');
            const overlay = document.querySelector('.page-overlay');
            if (configPage) configPage.classList.remove('active');
            if (aboutPage) aboutPage.classList.remove('active');
            if (overlay) overlay.classList.remove('active');
        }

        function createConfigPage() {
            const overlay = document.createElement('div');
            overlay.className = 'page-overlay';
            document.body.appendChild(overlay);
            overlay.addEventListener('click', (e) => { if (e.target === overlay) showDashboard(); });
            const configPage = document.createElement('div');
            configPage.className = 'config-page';
            configPage.innerHTML = `
                <header class="config-header">
                    <div class="config-header-left">
                        <button class="back-btn">
                            <svg class="icon-small" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                <line x1="19" y1="12" x2="5" y2="12"></line>
                                <polyline points="12 19 5 12 12 5"></polyline>
                            </svg>
                            <span>Voltar</span>
                        </button>
                        <h1 class="config-title">Configurações</h1>
                    </div>
                    <button class="save-btn">
                        <svg class="icon-small" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                            <path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"></path>
                            <polyline points="17 21 17 13 7 13 7 21"></polyline>
                            <polyline points="7 3 7 8 15 8"></polyline>
                        </svg>
                        <span>Salvar</span>
                    </button>
                </header>
                <div class="config-content">
                    <div class="config-section">
                        <div class="config-section-header">
                            <svg class="config-section-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                <rect x="2" y="3" width="20" height="14" rx="2" ry="2"></rect>
                                <line x1="2" y1="20" x2="22" y2="20"></line>
                            </svg>
                            <div class="config-section-text">
                                <h3>Status do Dispositivo</h3>
                            </div>
                        </div>
                        <div class="status-container">
                            <div class="status-item">
                                <svg class="status-item-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                    <path d="M5 12.55a11 11 0 0 1 14.08 0M1.42 9a16 16 0 0 1 21.16 0M8.53 16.11a6 6 0 0 1 6.94 0"></path>
                                </svg>
                                <span class="status-item-name">Wi-Fi</span>
                                <span class="status-item-badge connected">Conectado</span>
                            </div>
                        </div>
                    </div>
                    <div class="config-section">
                        <div class="config-section-header">
                            <svg class="config-section-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                <path d="M5 12.55a11 11 0 0 1 14.08 0M1.42 9a16 16 0 0 1 21.16 0M8.53 16.11a6 6 0 0 1 6.94 0"></path>
                            </svg>
                            <div class="config-section-text">
                                <h3>Configurações de Medição</h3>
                            </div>
                        </div>
                        <div class="config-field">
                            <label class="config-field-label">Unidade de Temperatura</label>
                            <select class="config-select" id="temperatureUnit">
                                <option value="celsius">Celsius (°C)</option>
                                <option value="fahrenheit">Fahrenheit (°F)</option>
                            </select>
                        </div>
                    </div>
                </div>
            `;
            document.body.appendChild(configPage);

        }

        function createAboutPage() {
            const overlay = document.querySelector('.page-overlay') || document.createElement('div');
            if (!document.querySelector('.page-overlay')) {
                overlay.className = 'page-overlay';
                document.body.appendChild(overlay);
                overlay.addEventListener('click', (e) => { if (e.target === overlay) showDashboard(); });
            }

            const aboutPage = document.createElement('div');
            aboutPage.className = 'config-page about-page'; // Reusing config-page styles
            aboutPage.innerHTML = `
                <header class="config-header">
                    <div class="config-header-left">
                        <button class="back-btn">
                            <svg class="icon-small" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                <line x1="19" y1="12" x2="5" y2="12"></line>
                                <polyline points="12 19 5 12 12 5"></polyline>
                            </svg>
                            <span>Voltar</span>
                        </button>
                        <h1 class="config-title">Sobre Nós</h1>
                    </div>
                </header>
                <div class="config-content">
                    <div class="config-section">
                        <div class="config-section-header">
                            <svg class="config-section-icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"></path>
                                <circle cx="12" cy="7" r="4"></circle>
                            </svg>
                            <div class="config-section-text">
                                <h3>Hydrosense Team</h3>
                                <p>Conheça a equipe por trás do projeto</p>
                            </div>
                        </div>
                        <div style="padding: 1rem 0;">
                            <p>O Hydrosense é um projecto criado pelo estudantes do ensino médio da 13ª classe do Instituto Médio Privado Politecnico do Huambo mais conhecida com Escola Politecnica do Huambo.</p>
                            <p style="margin-top: 0.5rem;">O projecto foi criado como trabalho de fim de curso para o requisito de tecnico médio.</p>
                            <p style="margin-top: 0.5rem; color: var(--text-tertiary); display: flex; align-items: center; gap: 0.5rem;">
                                <svg style="width: 1rem; height: 1rem;" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                                    <path d="M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0 1 18 0z"></path>
                                    <circle cx="12" cy="10" r="3"></circle>
                                </svg>
                                <span> <a href="https://www.google.com/maps/place/Cidade+Alta,+Huambo,+Angola" target="_blank">Cidade Alta, Huambo, Angola</a></span>
                            </p>
                            <br>
                            <h4>Desenvolvedores:</h4>
                            <ul style="list-style: none; margin-top: 0.5rem; padding: 0;">
                                <li style="display: flex; align-items: center; gap: 0.75rem; margin-bottom: 0.75rem; color: var(--text-secondary);">
                                    <span>Amorim Caculo</span>
                                    <a href="https://www.instagram.com/amorinold_older/" target="_blank" style="color: var(--text-tertiary); display: flex; align-items: center;" title="Instagram">
                                        <svg style="width: 1.25rem; height: 1.25rem;" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                                            <rect x="2" y="2" width="20" height="20" rx="5" ry="5"></rect>
                                            <path d="M16 11.37A4 4 0 1 1 12.63 8 4 4 0 0 1 16 11.37z"></path>
                                            <line x1="17.5" y1="6.5" x2="17.51" y2="6.5"></line>
                                        </svg>
                                    </a>
                                </li>
                                <li style="display: flex; align-items: center; gap: 0.75rem; margin-bottom: 0.75rem; color: var(--text-secondary);">
                                    <span>Marcier Rocha</span>
                                    <a href="https://www.instagram.com/marcierrocha7/" target="_blank" style="color: var(--text-tertiary); display: flex; align-items: center;" title="Instagram">
                                         <svg style="width: 1.25rem; height: 1.25rem;" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                                            <rect x="2" y="2" width="20" height="20" rx="5" ry="5"></rect>
                                            <path d="M16 11.37A4 4 0 1 1 12.63 8 4 4 0 0 1 16 11.37z"></path>
                                            <line x1="17.5" y1="6.5" x2="17.51" y2="6.5"></line>
                                        </svg>
                                    </a>
                                </li>
                                <li style="display: flex; align-items: center; gap: 0.75rem; margin-bottom: 0.75rem; color: var(--text-secondary);">
                                    <span>Onezimo Cambuta</span>
                                    <a href="https://www.instagram.com/onezimo_cambuta/" target="_blank" style="color: var(--text-tertiary); display: flex; align-items: center;" title="Instagram">
                                         <svg style="width: 1.25rem; height: 1.25rem;" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                                            <rect x="2" y="2" width="20" height="20" rx="5" ry="5"></rect>
                                            <path d="M16 11.37A4 4 0 1 1 12.63 8 4 4 0 0 1 16 11.37z"></path>
                                            <line x1="17.5" y1="6.5" x2="17.51" y2="6.5"></line>
                                        </svg>
                                    </a>
                                </li>
                            </ul>
                            <br>
                            <p style="font-size: 0.875rem; color: var(--text-tertiary);">Versão 1.0.0</p>
                            

                        </div>
                    </div>
                </div>
            `;
            document.body.appendChild(aboutPage);
        }

        function populateConfigForm() {
            const temperatureUnit = document.getElementById('temperatureUnit');
            if (temperatureUnit) temperatureUnit.value = appState.config.temperatureUnit;
        }

        function saveConfig() {
            const temperatureUnit = document.getElementById('temperatureUnit');
            if (temperatureUnit) appState.config.temperatureUnit = temperatureUnit.value;
            localStorage.setItem('aquaanalyzer-config', JSON.stringify(appState.config));
            alert('Configurações salvas com sucesso!');
            updateDashboard();
            showDashboard();
        }

        function updateDashboard() {
            const tempValueElement = document.getElementById('tempValue');
            if (tempValueElement) {
                let displayTemp = appState.currentTempCelsius;
                let unit = '°C';

                if (appState.config.temperatureUnit === 'fahrenheit') {
                    displayTemp = (appState.currentTempCelsius * 9 / 5) + 32;
                    unit = '°F';
                }

                tempValueElement.innerText = `${displayTemp.toFixed(1)}${unit}`;
            }
        }

        function loadConfigFromStorage() {
            const saved = localStorage.getItem('aquaanalyzer-config');
            if (saved) {
                try { appState.config = JSON.parse(saved); }
                catch (e) { console.error('Error loading config:', e); }
            }
        }

        // Status Status Logic
        function updateStatusBadge(id, status) {
            const el = document.getElementById(id);
            if (!el) return;
            
            el.className = 'status-badge'; // Reset
            
            if (status === 'Bom') {
                el.classList.add('status-good');
            } else if (status === 'Médio') {
                el.classList.add('status-medium');
            } else {
                el.classList.add('status-bad');
            }
            el.innerText = status;
        }

        function getPhStatus(val) {
            if (val >= 6.5 && val <= 8.5) return 'Bom'; // Recomendação OMS
            if ((val >= 6.0 && val < 6.5) || (val > 8.5 && val <= 9.0)) return 'Médio';
            return 'Baixo'; // Risco de corrosão ou baixa desinfeção
        }

        function getTurbidezStatus(val) {
            if (val <= 1.0) return 'Bom'; // Padrão OMS (Ideal < 1 NTU)
            if (val <= 5.0) return 'Médio'; // Aceitável estético (Até 5 NTU)
            return 'Baixo'; // Perigo biológico
        }

        function getTempStatus(val) {
             if (val > 0 && val < 20) return 'Bom'; // OMS: Ideal para evitar microorganismos (< 20°C)
             if (val >= 20 && val <= 25) return 'Médio'; // Aceitável mas em alerta para climas quentes
             return 'Baixo'; // OMS: > 25°C propício à bactéria Legionella e algas
        }

        async function fetchSensorData() {
            try {
                const response = await fetch('/api/sensor-data');
                const data = await response.json();
                
                appState.currentTempCelsius = data.temperatura;
                updateDashboard(); // Updates Temp with correct unit

                // Update Other Values
                if(document.getElementById('phValue')) document.getElementById('phValue').innerText = data.pH.toFixed(1);
                if(document.getElementById('turbidezValue')) document.getElementById('turbidezValue').innerText = data.turbidez.toFixed(1) + 'NTU';
                if(document.getElementById('timeValue')) document.getElementById('timeValue').innerText = data.ultimaAtualizacao;

                // Update Status Badges
                updateStatusBadge('phStatus', getPhStatus(data.pH));
                updateStatusBadge('turbidezStatus', getTurbidezStatus(data.turbidez));
                updateStatusBadge('tempStatus', getTempStatus(data.temperatura));


            } catch (e) {
                console.error("Fetch error", e);
            }
        }
    </script>
</body>

</html>
)rawliteral";
