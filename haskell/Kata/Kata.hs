module Kata.Kata (
  analyzeString,
  addDigit,
  hasSum
  ) where

import Data.List

analyzeString :: [Int] -> [[Int]]
analyzeString s = filter (\x -> length x > 0)
                  (snd (mapAccumL (\acc x -> hasSum acc x) [] s)) -- `using` parList rseq 

addDigit :: Int -> (Int,[Int]) -> (Int,[Int])
addDigit x (v,l) = (v+x, l++[x])

hasSum :: [(Int,[Int])] -> Int -> ([(Int,[Int])],[Int])
hasSum acc x = (a,b)
               where a=(filter (\v -> fst v < 10) (map (addDigit x) acc)) ++ [(x,[x])]
                     b=foldl (\u v -> if (fst v) == x && length (snd v) > 1
                                     then  (snd v)++[x]
                                     else u) [] acc

