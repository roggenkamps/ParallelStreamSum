module Kata.Kata (
  analyzeString,
  hasSum0,
  hasSum
  ) where

import Data.List

analyzeString :: [Int] -> [Int]
analyzeString s = let sm = hasSum0 s
                  in
                   case sm of
                     Nothing -> []
                     Just v -> (takeWhile (< v) s)++[v]

hasSum0 :: [Int] -> Maybe Int
hasSum0 [] = Nothing
hasSum0 (x0:x1:l) = hasSum (x0+x1) l

hasSum :: Int -> [Int] -> Maybe Int
hasSum _ [] = Nothing
hasSum i (h:l) = if i > 9 then Nothing
                 else if i == h then Just i
                      else hasSum (i+h) l
