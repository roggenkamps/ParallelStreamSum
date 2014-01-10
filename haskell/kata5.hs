-- kata5.hs
--
-- Steve Roggenkamp
--

import Data.Char

main :: IO ()

main = do
  let inStr = "8745648184845171326578518184151512461752149647129746915414816354846454" in
    iter inStr 1000000

iter :: [Char] -> Int -> IO ()
iter s 1 = analyzeString s
iter s c =
    do 
      analyzeString s
      iter s (c-1)

analyzeString :: [Char] -> IO ()
analyzeString [] = print ""
analyzeString s =
  do
    let x = hasSum0 "" s in
      if (fst x) /= "" then
        do
          print (fst x)
          analyzeString (tail s)
      else
        analyzeString (tail s)

    

hasSum0 :: [Char] -> [Char] -> ([Char],[Char])
hasSum0 "" [] = ("", "")
hasSum0 s [s0] = ("", "")
hasSum0 s [s0,s1] = ("", "")
hasSum0 s (s0:s1:r) = hasSum (digitToInt(s0)+digitToInt(s1)) [s0,s1] r

hasSum :: Int -> [Char] -> [Char] -> ([Char],[Char])
hasSum v s [] = ("","")
hasSum v s l = if v > 9
               then ("",l)
               else
	         let x = digitToInt (head l) in
                   if  v == x
                   then (s++[(head l)], tail(l))
	  	   else hasSum (v+x) (s++[(head l)]) (tail l)
                 