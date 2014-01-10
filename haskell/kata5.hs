-- kata5.hs
--
-- Steve Roggenkamp
--

import Data.Char
import GHC.Conc

main :: IO ()
main = do
  let inStr = "8745648184845171326578518184151512461752149647129746915414816354846454"
      count = 1000000 in
    print (iter (map digitToInt inStr) count)

iter :: [Int] -> Int -> [[Int]]
iter s 1 = analyzeString s
iter s c = par x (pseq y (x ++ y ))
           where x = analyzeString s  
                 y = iter s (c-1)

analyzeString :: [Int] -> [[Int]]
analyzeString [] =  [[]]
analyzeString s =
    let x = hasSum0 [] s in
      if (fst x) /= [] then
          [(fst x)] ++ analyzeString (tail s)
      else
        analyzeString (tail s)

hasSum0 :: [Int] -> [Int] -> ([Int],[Int])
hasSum0 [] [] = ([], [])
hasSum0 s [s0] = ([], [])
hasSum0 s (s0:s1:r) = hasSum (s0+s1) [s0,s1] r

hasSum :: Int -> [Int] -> [Int] -> ([Int],[Int])
hasSum v s [] = ([],[])
hasSum v s l = if v > 9
               then ([],l)
               else
	         let x = head l in
                   if  v == x
                   then (s++[(head l)], tail(l))
	  	   else hasSum (v+x) (s++[(head l)]) (tail l)
                 