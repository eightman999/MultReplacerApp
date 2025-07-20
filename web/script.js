/**
 * Multi-Replacer Web Application
 * JavaScript implementation for file text replacement functionality
 */

document.addEventListener('DOMContentLoaded', function() {
    
    // Application state
    let fileContent = '';
    let fileName = '';
    let modifiedContent = '';
    
    // DOM elements
    const fileInput = document.getElementById('file-input');
    const fileNameDisplay = document.getElementById('file-name');
    const addRowBtn = document.getElementById('add-row-btn');
    const executeBtn = document.getElementById('execute-btn');
    const replacementRows = document.getElementById('replacement-rows');
    const previewModal = document.getElementById('preview-modal');
    const previewContent = document.getElementById('preview-content');
    const modalCloseBtn = document.getElementById('modal-close-btn');
    const cancelBtn = document.getElementById('cancel-btn');
    const saveBtn = document.getElementById('save-btn');
    const replacementRowTemplate = document.getElementById('replacement-row-template');
    const langSelector = document.getElementById('lang-selector');

    let translations = {};
    let currentLang = 'ja';

    async function loadLanguage(lang) {
        const response = await fetch(`lang/${lang}.json`);
        translations = await response.json();

        document.querySelectorAll('[data-i18n]').forEach(el => {
            const key = el.getAttribute('data-i18n');
            if (translations[key]) {
                el.textContent = translations[key];
            }
        });

        document.querySelectorAll('[data-i18n-placeholder]').forEach(el => {
            const key = el.getAttribute('data-i18n-placeholder');
            if (translations[key]) {
                el.setAttribute('placeholder', translations[key]);
            }
        });

        document.title = translations['title'] || document.title;
    }
    
    // Initialize the application
    init();
    
    function init() {
        // Event listeners
        fileInput.addEventListener('change', handleFileSelect);
        addRowBtn.addEventListener('click', addReplacementRow);
        executeBtn.addEventListener('click', showPreview);
        modalCloseBtn.addEventListener('click', hidePreview);
        cancelBtn.addEventListener('click', hidePreview);
        saveBtn.addEventListener('click', saveModifiedFile);
        langSelector.addEventListener('change', () => {
            currentLang = langSelector.value;
            loadLanguage(currentLang);
        });
        
        // Event delegation for delete buttons
        replacementRows.addEventListener('click', handleDeleteRow);
        
        // Close modal when clicking outside
        previewModal.addEventListener('click', function(e) {
            if (e.target === previewModal) {
                hidePreview();
            }
        });
        
        // Add initial replacement row
        addReplacementRow();

        // Load default language
        loadLanguage(currentLang);
    }
    
    /**
     * Handles file selection and reading
     */
    function handleFileSelect(event) {
        const file = event.target.files[0];
        
        if (!file) {
            fileContent = '';
            fileName = '';
            fileNameDisplay.textContent = translations['no_file_selected'];
            executeBtn.disabled = true;
            return;
        }
        
        // Check if file is a text file
        if (!file.type.startsWith('text/') &&
            !file.name.match(/\.(txt|csv|json|xml|html|js|css|md|log|ini|conf|cfg)$/i)) {
            alert(translations['select_text_file']);
            fileInput.value = '';
            return;
        }
        
        const reader = new FileReader();
        
        reader.onload = function(e) {
            fileContent = e.target.result;
            fileName = file.name;
            fileNameDisplay.textContent = `${translations['selected_file']} ${fileName}`;
            executeBtn.disabled = false;
        };
        
        reader.onerror = function() {
            alert(translations['file_read_error']);
            fileInput.value = '';
        };
        
        reader.readAsText(file, 'UTF-8');
    }
    
    /**
     * Adds a new replacement row
     */
    function addReplacementRow() {
        const template = replacementRowTemplate.content.cloneNode(true);
        replacementRows.appendChild(template);

        // Apply translations to the newly added elements
        loadLanguage(currentLang);

        // Scroll to bottom to show new row
        replacementRows.scrollTop = replacementRows.scrollHeight;
    }
    
    /**
     * Handles deletion of replacement rows using event delegation
     */
    function handleDeleteRow(event) {
        if (event.target.classList.contains('delete-row-btn')) {
            const row = event.target.closest('.replacement-row');
            if (row) {
                row.remove();
                
                // Ensure at least one row remains
                if (replacementRows.children.length === 0) {
                    addReplacementRow();
                }
            }
        }
    }
    
    /**
     * Performs multiple, simultaneous replacements on a string.
     * This implementation avoids chaining and overlapping issues.
     */
    function multiReplace(content, replacements) {
        if (Object.keys(replacements).length === 0) {
            return content;
        }
        
        // Helper to escape special characters for use in a regex
        const escapeRegExp = (string) => {
            return string.replace(/[.*+?^${}()|[\\]\\]/g, '\\\\$&');
        };
        
        // Sort keys by length, descending, to handle overlapping matches correctly
        const sortedKeys = Object.keys(replacements).sort((a, b) => b.length - a.length);
        
        // Filter out empty keys
        const validKeys = sortedKeys.filter(key => key.length > 0);
        
        if (validKeys.length === 0) {
            return content;
        }
        
        // Create a single regex with the 'g' flag for global search
        const pattern = new RegExp(validKeys.map(escapeRegExp).join('|'), 'g');
        
        // Use a replacer function
        return content.replace(pattern, (matched) => {
            return replacements[matched];
        });
    }
    
    /**
     * Collects replacement rules from the UI
     */
    function collectReplacements() {
        const replacements = {};
        const rows = replacementRows.querySelectorAll('.replacement-row');
        
        rows.forEach(row => {
            const beforeInput = row.querySelector('.before-input');
            const afterInput = row.querySelector('.after-input');
            
            if (beforeInput && afterInput) {
                const beforeText = beforeInput.value.trim();
                const afterText = afterInput.value;
                
                if (beforeText) {
                    replacements[beforeText] = afterText;
                }
            }
        });
        
        return replacements;
    }
    
    /**
     * Shows the preview modal with the modified content
     */
    function showPreview() {
        if (!fileContent) {
            alert(translations['no_file_selected']);
            return;
        }
        
        const replacements = collectReplacements();
        
        if (Object.keys(replacements).length === 0) {
            alert(translations['no_replacements']);
            return;
        }
        
        // Apply replacements
        try {
            modifiedContent = multiReplace(fileContent, replacements);
            
            // Display in preview
            previewContent.textContent = modifiedContent;
            previewModal.style.display = 'flex';
            
            // Focus on the modal for better UX
            previewModal.focus();
            
        } catch (error) {
            alert(translations['processing_error'] + ' ' + error.message);
        }
    }
    
    /**
     * Hides the preview modal
     */
    function hidePreview() {
        previewModal.style.display = 'none';
        modifiedContent = '';
    }
    
    /**
     * Downloads the modified file
     */
    function saveModifiedFile() {
        if (!modifiedContent) {
            alert(translations['no_content_modified']);
            return;
        }
        
        try {
            // Create a blob with the modified content
            const blob = new Blob([modifiedContent], { type: 'text/plain;charset=utf-8' });
            
            // Create a temporary URL for the blob
            const url = URL.createObjectURL(blob);
            
            // Create a temporary anchor element for download
            const a = document.createElement('a');
            a.href = url;
            a.download = fileName || 'modified_file.txt';
            a.style.display = 'none';
            
            // Append to body, click, and remove
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            
            // Clean up the URL
            URL.revokeObjectURL(url);
            
            // Hide modal and show success message
            hidePreview();
            alert(translations['download_complete']);

        } catch (error) {
            alert(translations['download_error'] + ' ' + error.message);
        }
    }
    
    // Keyboard shortcuts
    document.addEventListener('keydown', function(e) {
        // Ctrl+Enter or Cmd+Enter to execute
        if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
            if (!executeBtn.disabled) {
                showPreview();
            }
            e.preventDefault();
        }
        
        // Escape to close modal
        if (e.key === 'Escape' && previewModal.style.display === 'flex') {
            hidePreview();
            e.preventDefault();
        }
        
        // Ctrl+A or Cmd+A to add new row (when not in input)
        if ((e.ctrlKey || e.metaKey) && e.key === 'a' && 
            !e.target.matches('input, textarea')) {
            addReplacementRow();
            e.preventDefault();
        }
    });
    
    // Prevent form submission if accidentally wrapped in a form
    document.addEventListener('submit', function(e) {
        e.preventDefault();
    });
    
    // Handle drag and drop for file input
    const fileHandler = document.getElementById('file-handler');
    
    fileHandler.addEventListener('dragover', function(e) {
        e.preventDefault();
        fileHandler.style.backgroundColor = '#e3f2fd';
    });
    
    fileHandler.addEventListener('dragleave', function(e) {
        e.preventDefault();
        fileHandler.style.backgroundColor = '';
    });
    
    fileHandler.addEventListener('drop', function(e) {
        e.preventDefault();
        fileHandler.style.backgroundColor = '';
        
        const files = e.dataTransfer.files;
        if (files.length > 0) {
            fileInput.files = files;
            handleFileSelect({ target: fileInput });
        }
    });
    
});