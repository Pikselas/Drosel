#pragma once
#ifndef _ALGORITHM_
#include<algorithm>
#endif
#ifndef _STRING_
#include<string>
#endif
#ifndef _VECTOR_
#include<vector>
#endif
namespace ksTools
{
   inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }

    // trim from end (in place)
    inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
    }

    // trim from both ends (in place)
    inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }
  inline void split_by_delms(const std::string& str, const std::string& delms,std::vector<std::string>& LIST)
  {
      auto flaggedIterStart = str.begin();
      auto flaggedIterEnd = std::find_first_of(str.begin(), str.end(), delms.begin(), delms.end());
      while (flaggedIterEnd != str.end())
      {
          std::string tempHolder;
          if (std::distance(flaggedIterStart, flaggedIterEnd) != 0)
          {
              std::for_each(flaggedIterStart, flaggedIterEnd, [&](const char& chr) {
                  tempHolder.push_back(chr);
                  });
              LIST.emplace_back(tempHolder);
          }
          flaggedIterStart = flaggedIterEnd + 1;
          flaggedIterEnd = std::find_first_of(flaggedIterStart, str.end(), delms.begin(), delms.end());
      }
      if (std::distance(flaggedIterStart, flaggedIterEnd) != 0)
      {
          std::string tmp;
          std::for_each(flaggedIterStart, flaggedIterEnd, [&](const char& chr) {
              tmp.push_back(chr);
              });
          LIST.emplace_back(tmp);
      }
  }
  inline std::vector<std::string> split_by_delms(const std::string& str,const std::string& delms) 
  {
   std::vector<std::string> LIST;
   split_by_delms(str, delms, LIST);
   return LIST;
 }
 inline std::vector<std::string> split_by_word(const std::string& str, const std::string& word)
 {
   std::vector<std::string> LIST;
   auto FlaggedItrFirst = str.begin();
   auto FlaggedItrLast = FlaggedItrFirst;
   auto word_length = std::distance(word.begin(),word.end()); 
   while(true)
   {
     FlaggedItrLast = std::search(FlaggedItrFirst,str.end(),word.begin(),word.end());
     if(FlaggedItrLast != str.end())
     {
       if(std::distance(FlaggedItrFirst,FlaggedItrLast) != 0)
       {
       std::string tmp;
       std::for_each(FlaggedItrFirst,FlaggedItrLast,[&](const char c){
         tmp.push_back(c);
       });
       LIST.emplace_back(tmp);
       }
       FlaggedItrFirst = FlaggedItrLast + word_length;
     }
     else
     {
       break;
     }
   }
   std::string tmp;
   std::for_each(FlaggedItrFirst,str.end(),[&](const char c){
     tmp.push_back(c);
   });
   LIST.emplace_back(tmp);
   return LIST;
 }

 inline auto split_by_word(const char* str, size_t str_size, const char* word, size_t word_size)
 {
     std::vector<std::vector<char>> LIST;
     size_t flaggedPoint = 0;
     for (size_t i = 0; i < str_size; i++)
     {
         if (i + word_size > str_size)
         {
             break;
         }
         else if (str[i] == word[0])
         {
             bool Found = true;
             for (size_t j = 1; j < word_size; j++)
             {
                 if (str[i + j] != word[j])
                 {
                     Found = false;
                     break;
                 }
             }
             if (Found)
             {
                 LIST.emplace_back(str + flaggedPoint, str + i);
                 flaggedPoint = (i += word_size);
             }
         }
     }
     LIST.emplace_back(str + flaggedPoint, str + str_size);
     return LIST;
 }
}