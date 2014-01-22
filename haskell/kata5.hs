-- kata5.hs
--
-- Steve Roggenkamp
--

import Control.Exception
import Control.Parallel.Strategies
import Data.Char
import Data.List
import System.Environment

main :: IO ()
main = do
  [cnt,bsz] <- getArgs 
  let inStr = "8745648184845171326578518184151512461752149647129746915414816354846454"
      count = read cnt
      batchsize = read bsz
      inInts = map digitToInt inStr
    in
      print (analyzeString batchsize (take count (cycle inInts)))

analyzeString :: Int -> [Int] -> [[Int]]
analyzeString b [] = []
analyzeString b s = (filter (\y -> not (null y))
                  (map (\x -> fst (hasSum0 [] x)) (take b (tails s)) `using` parList rseq )) ++ (analyzeString b (drop b s))

hasSum0 :: [Int] -> [Int] -> ([Int],[Int])
hasSum0 [] []       = ([], [])
hasSum0 s [s0]      = ([], [])
hasSum0 s (s0:s1:r) = hasSum (s0+s1) [s0,s1] r

hasSum :: Int -> [Int] -> [Int] -> ([Int],[Int])
hasSum v s [] = ([],[])
hasSum v s l  = if v > 9
                then ([],l)
                 else
	           let x = head l in
                       if  v == x
                       then (s++[(head l)], tail(l))
	  	       else hasSum (v+x) (s++[(head l)]) (tail l)
                 