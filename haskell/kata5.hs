-- kata5.hs
--
-- Steve Roggenkamp
--

import Data.Char
import Control.Parallel
import Control.Parallel.Strategies
import Control.Exception

main :: IO ()
main = do
  let inStr = "8745648184845171326578518184151512461752149647129746915414816354846454"
      count = 1000000 in
      iter (map digitToInt inStr) count

data SeqData = Active { val:: Int, str::[Int] }
             | Complete [Int]
             | NoSeq

data PData = PData { out :: [[Int]], seqs :: [SeqData] }

iter :: [Int] -> Int -> IO ()
iter s 1 = print (analyzeString s)
iter s c = (do print x; y)
           where x = analyzeString s
                 y = iter s (c-1)

completeSeq :: Int -> SeqData -> SeqData
completeSeq i (Active {val=v, str=s} ) =
   if v == i
   then Complete (s++[i])
   else if v > 9
        then NoSeq
        else Active { val = v+i, str=s++[i]}
completeSeq i (Complete s) = Complete s
completeSeq i NoSeq      = NoSeq

analyzeString :: [Int] -> [[Int]]
analyzeString [] =  [[]]
analyzeString s = out(foldl analyzeInt PData { out=[], seqs=[] } s)

analyzeInt :: PData -> Int -> PData
analyzeInt PData{ out=o, seqs=a } i  =
  let x = map (completeSeq i) (a ++ [Active { val = 0, str = []}])
  in
   PData { out  = o++(map extractSeqs (filter filterP0 x)),
           seqs = filter filterP1 x }

extractSeqs :: SeqData -> [Int]
extractSeqs (Active {val=_, str=_ }) = []
extractSeqs (Complete x)      = x
extractSeqs NoSeq             = []

filterP0 :: SeqData -> Bool
filterP0 (Active {val=_, str=_ }) = False
filterP0 (Complete x)      = True
filterP0 NoSeq             = False

filterP1 :: SeqData -> Bool
filterP1 (Active {val= _, str=_ }) = True
filterP1 (Complete x)      = False
filterP1 NoSeq             = False
