/**
 * Multi-Replace Function Implementation
 * Handles safe string replacement avoiding chaining and overlapping issues
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

/**
 * Performs multiple string replacements in a single pass.
 * This approach is safer than a simple loop because it avoids chaining
 * and handles overlapping patterns correctly.
 * 
 * @param source The original string to process
 * @param replacements Map of {find_string: replace_string} pairs
 * @return The string with all replacements applied
 */
std::string multiReplace(const std::string& source, const std::map<std::string, std::string>& replacements) {
    if (source.empty() || replacements.empty()) {
        return source;
    }
    
    std::stringstream result;
    size_t pos = 0;
    
    while (pos < source.length()) {
        std::string bestMatch;
        std::string replacement;
        size_t bestMatchLength = 0;
        
        // At current position, check all possible replacements
        // and find the longest match
        for (const auto& [find_str, replace_str] : replacements) {
            if (find_str.empty()) continue;
            
            // Check if this pattern matches at the current position
            if (pos + find_str.length() <= source.length()) {
                if (source.substr(pos, find_str.length()) == find_str) {
                    // This pattern matches, check if it's longer than current best
                    if (find_str.length() > bestMatchLength) {
                        bestMatch = find_str;
                        replacement = replace_str;
                        bestMatchLength = find_str.length();
                    }
                }
            }
        }
        
        if (bestMatchLength > 0) {
            // We found a replacement, apply it
            result << replacement;
            pos += bestMatchLength;
        } else {
            // No replacement found, copy the original character
            result << source[pos];
            pos++;
        }
    }
    
    return result.str();
}

/**
 * Alternative implementation using sorted keys for better performance
 * when dealing with many replacement patterns
 */
std::string multiReplaceOptimized(const std::string& source, const std::map<std::string, std::string>& replacements) {
    if (source.empty() || replacements.empty()) {
        return source;
    }
    
    // Create a vector of replacement pairs sorted by pattern length (descending)
    std::vector<std::pair<std::string, std::string>> sortedReplacements;
    for (const auto& [find_str, replace_str] : replacements) {
        if (!find_str.empty()) {
            sortedReplacements.emplace_back(find_str, replace_str);
        }
    }
    
    // Sort by length (descending), so longer patterns are checked first
    std::sort(sortedReplacements.begin(), sortedReplacements.end(),
              [](const auto& a, const auto& b) {
                  return a.first.length() > b.first.length();
              });
    
    std::stringstream result;
    size_t pos = 0;
    
    while (pos < source.length()) {
        bool found = false;
        
        // Check patterns in order of length (longest first)
        for (const auto& [find_str, replace_str] : sortedReplacements) {
            if (pos + find_str.length() <= source.length()) {
                if (source.substr(pos, find_str.length()) == find_str) {
                    // Found a match, apply replacement
                    result << replace_str;
                    pos += find_str.length();
                    found = true;
                    break; // Stop checking other patterns
                }
            }
        }
        
        if (!found) {
            // No replacement found, copy the original character
            result << source[pos];
            pos++;
        }
    }
    
    return result.str();
}

// Test function to demonstrate correctness
void testMultiReplace() {
    std::cout << "Testing multiReplace function:\n\n";
    
    // Test 1: Basic replacement
    {
        std::string text = "Hello world, hello universe";
        std::map<std::string, std::string> rules = {
            {"hello", "hi"},
            {"world", "earth"}
        };
        
        std::string result = multiReplace(text, rules);
        std::cout << "Test 1 - Basic replacement:\n";
        std::cout << "Original: " << text << "\n";
        std::cout << "Result:   " << result << "\n";
        std::cout << "Expected: Hello earth, hi universe\n\n";
    }
    
    // Test 2: Overlapping patterns (longer should win)
    {
        std::string text = "caterpillar and cat";
        std::map<std::string, std::string> rules = {
            {"cat", "dog"},
            {"caterpillar", "butterfly"}
        };
        
        std::string result = multiReplace(text, rules);
        std::cout << "Test 2 - Overlapping patterns:\n";
        std::cout << "Original: " << text << "\n";
        std::cout << "Result:   " << result << "\n";
        std::cout << "Expected: butterfly and dog\n\n";
    }
    
    // Test 3: Chaining prevention
    {
        std::string text = "a b c";
        std::map<std::string, std::string> rules = {
            {"a", "b"},
            {"b", "c"}
        };
        
        std::string result = multiReplace(text, rules);
        std::cout << "Test 3 - Chaining prevention:\n";
        std::cout << "Original: " << text << "\n";
        std::cout << "Result:   " << result << "\n";
        std::cout << "Expected: b c c (NOT c c c)\n\n";
    }
    
    // Test 4: Empty replacements
    {
        std::string text = "remove this and this";
        std::map<std::string, std::string> rules = {
            {"this", ""},
            {" and ", " & "}
        };
        
        std::string result = multiReplace(text, rules);
        std::cout << "Test 4 - Empty replacements:\n";
        std::cout << "Original: " << text << "\n";
        std::cout << "Result:   " << result << "\n";
        std::cout << "Expected: remove  & \n\n";
    }
    
    // Test 5: Performance comparison
    {
        std::string text = "This is a test string with many words to replace in a typical text file.";
        std::map<std::string, std::string> rules = {
            {"test", "sample"},
            {"string", "text"},
            {"many", "several"},
            {"words", "terms"},
            {"typical", "standard"}
        };
        
        std::string result1 = multiReplace(text, rules);
        std::string result2 = multiReplaceOptimized(text, rules);
        
        std::cout << "Test 5 - Performance comparison:\n";
        std::cout << "Original: " << text << "\n";
        std::cout << "Result 1: " << result1 << "\n";
        std::cout << "Result 2: " << result2 << "\n";
        std::cout << "Results match: " << (result1 == result2 ? "YES" : "NO") << "\n\n";
    }
}

int main() {
    testMultiReplace();
    return 0;
}