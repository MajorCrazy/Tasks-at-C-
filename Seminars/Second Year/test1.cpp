#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    int relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        for (auto this_word : words)
            documents_[this_word].insert(document_id);
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        Query query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.relevance > rhs.relevance;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:

    struct Query {
        set<string> minus_words;
        set<string> plus_words;
    };

    map<string, set<int>> documents_;

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if (CheckMinus(word)) {
                query_words.plus_words.insert(word);
            }
            else {
                query_words.minus_words.insert(word.substr(1));
            }
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const Query& query_words) const {
        vector<Document> matched_documents;
        map<int, int> relevance;
        for (const auto& plus_word : query_words.plus_words) {
            if (documents_.count(plus_word)) {
                for (const auto& documents_id : documents_.at(plus_word)) {
                    relevance[documents_id] += 1;
                }
            }

            for (const auto& id : DocumentWithoutMinusWords(documents_, query_words.minus_words)) {
                if (relevance.first == id) {
                    vector<Document> matched_documents.push_back({ id, relevance.second };)//relevance.at(id)
                }
            }
            return matched_documents;
        }

        bool CheckMinus(const string & query) const {
            return !(query[0] == '-');
        }

        set<int> DocumentWithoutMinusWords(const map<string, set<int>>&doc, const set<string>&minus_words) const {
            set<int> search;
            for (const auto& current : doc) {
                if (!minus_words.count(current.first))
                    search.insert(current.second);
            }
            return search;
        }

        SearchServer CreateSearchServer() {
            SearchServer search_server;
            search_server.SetStopWords(ReadLine());
            const int document_count = ReadLineWithNumber();
            for (int document_id = 0; document_id < document_count; ++document_id) {
                search_server.AddDocument(document_id, ReadLine());
            }

            return search_server;
        }
    }
};

    int main() {
        const SearchServer search_server = CreateSearchServer();

        const string query = ReadLine();
        for (const auto& para : search_server.FindTopDocuments(query)) {
            cout << "{ document_id = "s << para.id << ", relevance = "s << para.relevance << " }"s << endl;
        }
    }
