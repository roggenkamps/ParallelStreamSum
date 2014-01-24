-- kata5.hs
--
-- Steve Roggenkamp
--

import Control.Exception
import Control.Parallel.Strategies
import Data.Char
import Data.List
import System.Environment
import Kata.Kata

main :: IO ()
main = do
  [cnt] <- getArgs 
  let inStr = "8745648184845171326578518184151512461752149647129746915414816354846454"
      -- inStr = "111111111911248124617"
      count = read cnt
      inInts = map digitToInt inStr
    in
   (print
           (filter (\x -> (not . null) x)
            (map analyzeString ((take count . tails . cycle) inInts))
           )
          )
