CandleFlickerSimulator
======================

Super-realistic candle flicker algorithms that can run on even the lowest-end microcontrolers out there.
Comes in two versions. The first doubles as a jitter-free rainbow nightlight and uses the generation 1 algorithm for flickering,
wheras the second does not yet include the rainbow feature yet,but uses the generation 2 algorithm which attempts to model
wind gust patterns and compile to around 200 bytes of ram.

The RNG in gen 1 is a custom one I stumbled on that generates very good flickering without much filtering
(it is not a true RNG) wheras gen 2 uses my XABC rng design to drive a more physically-inspired cande simulation.
