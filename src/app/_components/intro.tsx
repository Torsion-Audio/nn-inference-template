import { CMS_NAME } from "@/lib/constants";

export function Intro() {
  return (
    <section className="flex-col md:flex-row flex items-center md:justify-between mt-16 mb-16 md:mb-12">
      <div>
        <h1 className="text-4xl md:text-6xl font-bold tracking-tighter leading-tight md:pr-8">
          nn-inference-template
        </h1>
        <h2 className="text-2xl md:text-4xl font-light tracking-tight mt-4 md:mt-2">
          ADC23 Talk: Real-time inference of neural networks - a practical approach for DSP engineers
        </h2>
      </div>
    </section>
  );
}
