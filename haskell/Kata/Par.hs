module Kata.Par (
  analyzeString,
  analyzeInt,
  completeSeq,
  filter1,
  filter2
  )
       where

import Control.Exception
--import Control.Parallel
--import Control.Parallel.Strategies
import Data.Char
import Data.Maybe

data SeqData = SD { val:: Int, str::[Int] }
             | Out ([Int]) deriving (Show)

analyzeString :: ([[Int]],[SeqData]) -> [Int] -> IO ()
analyzeString (o,sd) [] = print o
analyzeString (o,sd) s = 
  let (o1,sd1) = analyzeInt (o,sd) (head s)
    in
      if not (null o1) then
        do
          print (head o1)
          analyzeString ([],sd1) (tail s)
      else
        analyzeString (o1,sd1) (tail s)

analyzeInt :: ([[Int]],[SeqData]) -> Int -> ([[Int]],[SeqData])
analyzeInt (o,a) i  =
  let x = map (completeSeq i) (a ++ [SD { val = 0, str = []}])
      in ((o++(filter2 x)),(filter1 x))
   
completeSeq :: Int -> SeqData -> Maybe SeqData
completeSeq i (SD {val=v, str=s} ) =
   if v == i
   then  Just (Out(s++[i]))
   else if v > 9
        then Nothing
        else Just (SD { val = v+i, str=s++[i]})
completeSeq i x = Nothing

filter1 :: [Maybe SeqData] -> [SeqData]
filter1 l = [SD{val=v,str=s} | Just (SD {val=v, str=s}) <- l ]

filter2 :: [Maybe SeqData] -> [[Int]]
filter2 l = [x | Just (Out x) <- l ]
