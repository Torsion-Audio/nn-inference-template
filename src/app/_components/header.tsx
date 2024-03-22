import Link from "next/link";

const Header = () => {
  return (
    <div>
      <h2 className="text-2xl md:text-4xl font-bold tracking-tight md:tracking-tighter leading-tight mb-4 mt-8">
        <Link href="/" className="hover:underline">
          nn-inference-template
        </Link>
      </h2>
      <h3 className="text-xl md:text-3xl font-medium tracking-tight leading-tight mb-20">
        ADC23 Talk: Real-time inference of neural networks: a practical approach for DSP engineers
      </h3>
    </div>
  );
};

export default Header;
