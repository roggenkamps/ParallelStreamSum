-- kata5.hs
--
-- Steve Roggenkamp
--

import Control.Exception
import Control.Parallel
import Control.Parallel.Strategies
import Data.Char
import Data.Maybe
import System.Environment
import System.IO
import Kata.Par

main :: IO ()
main = do
  [cnt] <- getArgs 
  let inStr = "8745648184845171326578518184151512461752149647129746915414816354846454"
      inInts = map digitToInt inStr
      count = read cnt
      in analyzeString ([],[])  (take count (cycle inInts))

